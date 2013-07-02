using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace HookProcessor
{
    public class KeyInfoEventArg: EventArgs
    {
        public int Message { get; set; }
        public int KeyCode { get; set; }
        public IntPtr Hwnd { get; set; }
    }
}
