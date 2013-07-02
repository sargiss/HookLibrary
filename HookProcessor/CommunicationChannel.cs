using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace HookProcessor
{
    public abstract class CommunicationChannel
    {
        private readonly int _descSize = Marshal.SizeOf(typeof(CommunicationMessageDesc));

        public Action<CommunicationMessageFull> Recieved;

        public abstract void StartListen();
        public abstract void StopListen();

        public void OnRecieved(byte[] data, int count = 0)
        {
            if (count == 0)
                count = data != null ? data.Length : 0;
            var recieved = Recieved;
            if (recieved != null)
            {
                IntPtr intPtr = Marshal.AllocHGlobal(_descSize);
                Marshal.Copy(data, 0, intPtr, _descSize);
                CommunicationMessageDesc msgDesc = (CommunicationMessageDesc)Marshal.PtrToStructure(intPtr, typeof(CommunicationMessageDesc));
                Marshal.FreeHGlobal(intPtr);
                var additionalData = new byte[count - _descSize];
                Array.Copy(data, _descSize, additionalData, 0, additionalData.Length); 
                CommunicationMessageFull msg = new CommunicationMessageFull
                {
                    MessageInfo = msgDesc,
                    AdditionalInfo = additionalData
                };
                recieved(msg);
            }
        }
    }
}
