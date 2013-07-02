using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;

namespace HookProcessor
{
    public class PostMessageHandle : IMessageHandle
    {
        private int _windowHandle = GetConsoleWindow().ToInt32();
        private StreamWriter sw;

        public PostMessageHandle()
        {
            sw = File.CreateText(@"C:\hook_gui_log.txt");
        }

        public HookType HookType
        {
            get { return HookType.WH_CALLWNDPROC; }
            //get { return HookType.WH_GETMESSAGE; }
        }

        public void ProcessMessage(CommunicationMessageFull messageInfo)
        {
            //sw.WriteLine("get msg = " + messageInfo.MessageInfo.Message);
            if (_windowHandle != messageInfo.MessageInfo.HWnd.ToInt32())
                Console.WriteLine(messageInfo.MessageInfo.Message);
                //sw.WriteLine("wnd = " + _windowHandle);
        }

        private readonly WinApi.Messages[] TargetMessages = new WinApi.Messages[]
            {
                WinApi.Messages.WM_KEYDOWN, 
                WinApi.Messages.WM_KEYUP
            };

        public IEnumerable<WinApi.Messages> HadledMessages
        {
            get
            {
                return new WinApi.Messages[]
                {
                    WinApi.Messages.WM_KEYDOWN,
                    WinApi.Messages.WM_COPYDATA
                };
            }
        }

        [DllImport("kernel32.dll")]
        static extern IntPtr GetConsoleWindow();
    }
}
