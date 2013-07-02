#include "stdafx.h"
#include "MessageFilter.h"
#include "DataSenderPipes.h"
#include <iostream>
#include "Lazy.h"
#include "global.h"
#include "DataSenderMsg.h"
#include <iomanip>

extern "C" __declspec(dllexport) LRESULT CALLBACK CallWindProc(_In_  int nCode, _In_  WPARAM wParam, _In_  LPARAM lParam)
{
	static DWORD currentProc = ::GetCurrentProcessId(); 
	static CDataSenderPipes sender;

	if (nCode == HC_ACTION)
	{
		try{
			CWPSTRUCT* pMessage = (CWPSTRUCT*)lParam;
			
			if (g_hookInfo.processId != currentProc)
			{
				DataToSend<> dataToSend;
				dataToSend.hook_type = WH_CALLWNDPROC;
				dataToSend.message = pMessage->message;
				dataToSend.hWnd = pMessage->hwnd;
				sender.Send(dataToSend);
			}
			else
				logger<<"Same proces\n";
		}
		catch(std::runtime_error e){
			logger<<e.what();
		}
		catch(...){
			logger<<"Unexpected error.";
		}
	}
	return 0;//CallNextHookEx(NULL, nCode, wParam, lParam);
}

extern "C" __declspec(dllexport) LRESULT CALLBACK GetMsgProc(_In_  int nCode, _In_  WPARAM wParam, _In_  LPARAM lParam)
{
	/*static CDataSenderPipes<DataToSend<MSG>> sender;

	logger<<"message recieved...";

	if (nCode == HC_ACTION && wParam == PM_REMOVE)
	{
		try{
			MSG* pMessage = (MSG*)lParam;
			DataToSend<MSG> dataToSend;
			dataToSend.hook_type = WH_GETMESSAGE;
			dataToSend.data = *pMessage;
			//sender.Send(dataToSend);
		}
		catch(std::runtime_error e){
			logger<<e.what();
		}
		catch(...){
			logger<<"error";
		}
	}*/
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}