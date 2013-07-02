using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace HookProcessor
{
    public interface IMessageHandle
    {
        HookType HookType { get; }
        IEnumerable<WinApi.Messages> HadledMessages { get; }
        void ProcessMessage(CommunicationMessageFull message);
    }
}
