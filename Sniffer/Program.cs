using System;
using System.Collections.Generic;
using System.Linq;
using HookProcessor;
using System.Threading;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.IO.MemoryMappedFiles;

namespace Sniffer
{
    public class Program
    {
        public static void Main(string[] args)
        {
            var keyPressHandle = new PostMessageHandle();
            /*keyPressHandle.KeyMsgRecieved += (sender, keyInfo) =>
            {
                Console.WriteLine(keyInfo.KeyCode + "  " + keyInfo.Message);
            };*/
            Console.WriteLine(Process.GetCurrentProcess().Id);
            
            using (var hooker = new Hook(keyPressHandle))
            {
                hooker.StartSniffing();
                Console.WriteLine("start");
                Console.ReadLine();
                Console.WriteLine("exit");
            }
            
            return;
           //*/
            const string DllName = "MessageFilter.dll";
            var proc = "_CallWindProc@12";
            var mmf = WriteSettings();

            if (System.IO.File.Exists(@"C:\sniffer_log.txt"))
                System.IO.File.Delete(@"C:\sniffer_log.txt");

            var hModule = WinApi.LoadLibrary(DllName);

            var recieverPntr = WinApi.GetProcAddress(hModule, proc);
            var onMessageRecieved = (WinApi.CallWndProc)Marshal.GetDelegateForFunctionPointer(recieverPntr, typeof(WinApi.CallWndProc));

            CommunicationChannel channel = new PipeChannel();

            Console.WriteLine(Process.GetCurrentProcess().Id);

            int k = 0;
            channel.Recieved += new Action<CommunicationMessageFull>(b =>
            {
                Console.WriteLine("Recieved: {0}, {1}, {2}", 
                    b.MessageInfo.HWnd, b.MessageInfo.Message, b.AdditionalInfo.Length);
            });
            channel.StartListen();

            List<Thread> threads = new List<Thread>();
            for (int th = 0; th < 10; th++)
            {
                threads.Add(new Thread((o) =>
                {
                    int n = (int)o;
                    
                    for (int i = 0; i < 10; i++)
                    {   
                        try
                        {
                            WinApi.CWPSTRUCT data = new WinApi.CWPSTRUCT();
                            data.hwnd = new IntPtr(n);
                            data.message = i;
                            var pntr = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(WinApi.CWPSTRUCT)));
                            Marshal.StructureToPtr(data, pntr, true);
                            onMessageRecieved.Invoke(0, IntPtr.Zero, pntr);
                            Marshal.FreeHGlobal(pntr);
                            //Console.WriteLine("send: " + n + " i=" + i);
                        }
                        catch (Exception e)
                        {
                            Console.WriteLine(e.Message);
                        }
                    }
                    Console.WriteLine("send all: " + n);
                }));
                threads[th].Start(th);
            }
            
            
            //threads.ForEach(t => t.Join());

            Console.ReadLine();
            channel.StopListen();
            mmf.Dispose();
        }

        private static MemoryMappedFile WriteSettings()
        {
            const string SettingFileName = "HookMmfSettings";

            HookInfo inf = new HookInfo();
            inf.ProcessId = 0; // System.Diagnostics.Process.GetCurrentProcess().Id;
            inf.MessagesToHook = new[]{
                (int)WinApi.Messages.WM_COPYDATA,
                (int)WinApi.Messages.WM_KEYDOWN
            };
            return HookInfoSerializator.WriteToFile(SettingFileName, inf);
        }
    }
}
