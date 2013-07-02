using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.Pipes;
using System.IO;
using System.Threading;
using System.Diagnostics;

namespace HookProcessor
{
    public class PipeChannel : CommunicationChannel
    {
        private const string PipeName = "HookCommunicationChannel";
        private Semaphore _writeBlock = new Semaphore(0, NumberOfPipeInst, "SyncHookWritingSemaphore");
        private const int MaxBufferSize = 128;
        private const int NumberOfPipeInst = 15;

        private volatile bool _stopListen = false;

        public override void StartListen()
        {
            for (var i = 0; i < NumberOfPipeInst; i++)
                StartListen(null);
            _writeBlock.Release(NumberOfPipeInst);
        }

        public void StartListen(NamedPipeServerStream pipeServer)
        {
            if (_stopListen)
            {
                if (pipeServer != null)
                    pipeServer.Dispose();
                return;
            }

            try
            {
                if (pipeServer == null)
                {
                    pipeServer = new NamedPipeServerStream(PipeName, PipeDirection.InOut, NumberOfPipeInst,
                                  PipeTransmissionMode.Byte, PipeOptions.WriteThrough | PipeOptions.Asynchronous);
                }

                pipeServer.BeginWaitForConnection(OnConnectToClient, pipeServer);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }

        public override void StopListen()
        {
            _stopListen = true;
        }

        private void OnConnectToClient(IAsyncResult result)
        {
            var pipe = (NamedPipeServerStream)result.AsyncState;

            try
            {
                pipe.EndWaitForConnection(result);

                var msg = new byte[MaxBufferSize];
                var count = pipe.Read(msg, 0, MaxBufferSize);
                
                pipe.Disconnect();
                StartListen(pipe);
                _writeBlock.Release(1);
          
                OnRecieved(msg, count);
            }
            catch (Exception ex)
            {
                Console.WriteLine("Read from pipe error: " + ex.Message);
            }           
        }
    }
}
