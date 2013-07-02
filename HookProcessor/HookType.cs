using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace HookProcessor
{
    public enum HookType : int
    {
        //WH_KEYBOARD = 2,
        WH_CALLWNDPROC = 4,
        //WH_CALLWNDPROCRET = 12,
        WH_GETMESSAGE = 3
    }
}
