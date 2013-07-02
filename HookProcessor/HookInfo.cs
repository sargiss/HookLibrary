using System;
using System.Collections.Generic;
using System.Linq;
using System.IO.MemoryMappedFiles;

namespace HookProcessor
{
    public struct HookInfo
    {
        private int[] _messageToHook;

        public int ProcessId;
        public int[] MessagesToHook
        {
            get { return _messageToHook ?? new int[0]; }
            set { _messageToHook = value; }
        }
    }

    unsafe static public class HookInfoSerializator
    {
        private static readonly int ItemSize = sizeof(int);

        static public MemoryMappedFile WriteToFile(string fileName, HookInfo info)
        {
            var mmf = MemoryMappedFile.CreateNew(fileName, ItemSize * (2 + info.MessagesToHook.Length));
            var mmv = mmf.CreateViewAccessor();
            byte* buffer = null;
            mmv.SafeMemoryMappedViewHandle.AcquirePointer(ref buffer);
            int* target = (int*)buffer;

            WriteInt(ref target, info.ProcessId);
            WriteArray(ref target, info.MessagesToHook);

            mmv.Dispose();
            return mmf;
        }

        static void WriteInt(ref int* target, int val)
        {
            *target = val;
            target++;
        }

        static void WriteArray(ref int* target, int[] data)
        {
            WriteInt(ref target, data.Length);

            foreach (int val in data)
            {
                WriteInt(ref target, val);
            }
        }
    }
}
