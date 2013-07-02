using System;
using System.Runtime.InteropServices;

namespace HookProcessor
{
    [StructLayout(LayoutKind.Sequential)]
    public struct CommunicationMessageDesc
    {
        public WinApi.Messages Message;
        public HookType HookType;
        public IntPtr HWnd;
    }

    public class CommunicationMessageFull
    {
        public CommunicationMessageDesc MessageInfo;
        public byte[] AdditionalInfo;
    }
}
