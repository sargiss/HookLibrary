using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.InteropServices;
using System.IO.Pipes;
using System.Threading;
using System.Reflection;
using System.IO;
using System.IO.MemoryMappedFiles;

namespace HookProcessor
{
    public class Hook: IDisposable
    {
        readonly private Dictionary<HookType, string> Callbacks = new Dictionary<HookType, string>()
        {
            { HookType.WH_CALLWNDPROC, "_CallWindProc@12"},
            { HookType.WH_GETMESSAGE, "_GetMsgProc@12"}
        };

        private const string Log = @"C:\sniffer_log.txt";
        private const string DllName = "MessageFilter.dll";
        private const string SettingFileName = "HookMmfSettings";

        private IntPtr _hModule;
        private List<IntPtr> _hookIds;
        private CommunicationChannel _channel;
        private MemoryMappedFile _settingFile = null;
        
        Dictionary<HookType, IMessageHandle> _handles = new Dictionary<HookType,IMessageHandle>();

        public Hook(params IMessageHandle[] messageHandles)
        {
            _hookIds = new List<IntPtr>();
            _channel = new PipeChannel();

            if (File.Exists(Log))
                File.Delete(Log);


            foreach(IMessageHandle handle in messageHandles)
            {
                _handles.Add(handle.HookType, handle);
                _channel.Recieved += handle.ProcessMessage;
            }

            _hModule = WinApi.LoadLibrary(DllName);
            GetLastError(_hModule);
        }

        public void StartSniffing()
        {
            _channel.StartListen();
            _settingFile = WriteSettings();

            if (_hookIds.Count == 0)
            {
                foreach (var handle in _handles)
                {
                    var recieverPntr = WinApi.GetProcAddress(_hModule, Callbacks[handle.Key]);
                    var onMessageRecieved = (WinApi.CallWndProc)Marshal.GetDelegateForFunctionPointer(recieverPntr, typeof(WinApi.CallWndProc));
                    var hookId = WinApi.SetWindowsHookEx((int)handle.Key, onMessageRecieved, _hModule, 0);
                    GetLastError(hookId);
                    _hookIds.Add(hookId);
                }
            }
        }

        public void StopSniffing()
        {
            foreach (var hookId in _hookIds)
            {
                Console.WriteLine(WinApi.UnhookWindowsHookEx(hookId));
            }
            _hookIds.Clear();
            _channel.StopListen();
            _settingFile.Dispose();
            WinApi.CloseHandle(_hModule);
        }

        public void Dispose()
        {
            StopSniffing();
        }

        private MemoryMappedFile WriteSettings()
        {
            HookInfo inf = new HookInfo();
            inf.ProcessId = System.Diagnostics.Process.GetCurrentProcess().Id;
            inf.MessagesToHook = _handles.SelectMany(h => h.Value.HadledMessages).Cast<int>().ToArray();
            return HookInfoSerializator.WriteToFile(SettingFileName, inf);
        }

        private void GetLastError(IntPtr handle)
        {
            if (handle == IntPtr.Zero)
            {
                int errorCode = Marshal.GetLastWin32Error();
                throw new Win32Exception(errorCode);
            }
        }
    }
}
