#pragma once
#include <windows.h> 
#include "DataSender.h"
#include "resource.h"
#include "WinMutex.h"
#include "global.h"

class CDataSenderPipes
{
public:
	CDataSenderPipes(void)
	{
		m_readyEvent = ::CreateEventA(NULL, true, false, NULL);
		::LoadResourceStringW(IDS_PIPE, m_pipeName);
		::LoadResourceStringA(IDS_WRITEMUTEX, m_syncName);
		
		/*
		wchar_t name[256];
		name[255] = '\0';
		GetModuleFileName(NULL, name, 255);
		std::wstring ws(name);
		std::string s(ws.begin(), ws.end());
		is_exp = (s.find("conhost") != string::npos);
		logger<<::GetCurrentProcessId()<<" process: "<<s<<'\n';
		*/

		m_syncSemaphore = ::OpenSemaphoreA(SYNCHRONIZE, false, m_syncName);
		if (!m_syncSemaphore)
			LogLastError("Creating semaphore: ");
		
		::SetEvent(m_readyEvent);
	}

	~CDataSenderPipes(void)
	{
		::CloseHandle(m_readyEvent);
		::CloseHandle(m_syncSemaphore);
	}

	template <class T>
	void Send(const DataToSend<T>& data)
	{
		Send(data, sizeof(DataToSend<T>));
	}

	template <class T>
	void Send(const DataToSend<T>& data, int size)
	{
		::WaitForSingleObject(m_readyEvent, INFINITE);
		
		if(::WaitForSingleObject(m_syncSemaphore, 200)==WAIT_OBJECT_0)
		{
			if (HANDLE hPipe = TryToConnect())
			{
				try{		
					DWORD cbWritten;
					if (!::WriteFile(hPipe, &data, size, &cbWritten, NULL))
						::LogLastError("Writing to pipe: ");
				}
				catch(...){}
				CloseHandle(hPipe);
			}
		}
		else
			logger<<"Refused.\n";
	}

private:
	HANDLE TryToConnect()
	{
		HANDLE hPipe = NULL;
		while (1) 
		{ 
			hPipe = CreateFile( 
			 m_pipeName,   // pipe name 
			 GENERIC_WRITE,  // read and write access 
			 0,              // no sharing 
			 NULL,           // default security attributes
			 OPEN_EXISTING,  // opens existing pipe 
			 0,              // default attributes 
			 NULL);          // no template file 

			if (hPipe != INVALID_HANDLE_VALUE) 
				break;
 
			DWORD error = GetLastError();
			if (error != ERROR_PIPE_BUSY) 
			{
				::LogLastError(error, "Creating pipe: ");
				return NULL;
			}
			
			if (!WaitNamedPipeW(m_pipeName,
				NMPWAIT_WAIT_FOREVER)) 
			{
				::LogLastError();
				return NULL;
			}
			
		} 
		DWORD dwMode = PIPE_READMODE_BYTE;
		::SetNamedPipeHandleState(hPipe, &dwMode, NULL, NULL);
		return hPipe;
	}

	HANDLE m_readyEvent;
	wchar_t m_pipeName[MAX_SIZE];
	char m_syncName[MAX_SIZE];
	HANDLE m_syncSemaphore;
	int is_exp;
};

