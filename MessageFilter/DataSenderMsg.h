#pragma once
#include <windows.h> 
#include "DataSender.h"
#include "resource.h"
#include "WinMutex.h"
#include "global.h"
#include "CallBackWrapper.h"

#define MAX_SIZE 256
#define QUEUE_SIZE 40

template <class T>
class CDataSenderMsg : public CDataSender<T>
{
public:
	CDataSenderMsg(void):m_sender(NULL), m_currentPosition(0), m_lastPosition(0), m_stop(0)
	{
		m_readyEvent = ::CreateEventA(NULL, true, false, NULL);
		
		LoadResourceStringA(IDS_WRITEMUTEX, m_mutexName);
		//m_writeSyncMutex = new CWinMutex(m_mutexName, true);

		LoadResourceStringA(IDS_WNDCLASSNAME, m_wndName);
		strcpy(m_wndName, "Hooker.WindowReciever");
		FindTargetWindow();

		::InitializeCriticalSection(&queueSync);

		GetWindowThreadProcessId(m_hWnd, &m_wndPid);
		logger<<"WND_PID="<<m_wndPid<<'\n';
		m_pid = ::GetCurrentProcessId();
		logger<<"PID="<<m_pid<<'\n';
		if (m_pid != m_wndPid)
			return;
		m_semaphore = ::CreateSemaphoreA(NULL, QUEUE_SIZE, QUEUE_SIZE, NULL);
		m_mutex = ::CreateMutexA(NULL, false, NULL);
		m_sender = ::CreateThread(NULL, 0, DoSendWrapper, reinterpret_cast<LPVOID>(this), 0, NULL);
	}

	~CDataSenderMsg(void)
	{
		CloseHandle(m_sender);

		m_stop = true;

		

		logger<<"stop..."<<m_pid<<'\n';

		if (m_pid != m_wndPid)
			return;

		if (::WaitForSingleObject(m_sender, 5000))
			logger<<"stopped "<<m_pid<<'\n';
		else{
			::TerminateThread(m_sender, 0);
			logger<<"killed "<<m_pid<<'\n';
		}
		
		
		CloseHandle(m_semaphore);
		CloseHandle(m_mutex);
		
		try{
		for(int i=0; i<QUEUE_SIZE; i++)
		{
			if (m_messageQueue[i])
				delete[] m_messageQueue[i];
		}
		}catch(...)
		{
			//logger<<"clearing error\n";
		}
	}

	void Send(const T& data)
	{
		Send(data, sizeof(T));
	}

	void Send(const T& data, int size)
	{
		//logger<<"try to send"<<m_pid<<'\n'
		if (m_wndPid == m_pid)
			::WaitForSingleObject(m_readyEvent, 5000);

		if (!m_hWnd)
			return;
		
		//logger<<"SEND:"<<m_pid<<'\n';

		if (m_wndPid != m_pid){
			return;
		}

		if(::WaitForSingleObject(m_semaphore, 80)==WAIT_OBJECT_0)
		{	
			BYTE* pData = new BYTE[size];
			memcpy(pData, &data, size);
			::WaitForSingleObject(m_mutex, INFINITE);
			//::EnterCriticalSection(&queueSync);
			m_messageQueue[m_lastPosition] = pData;
			if (++m_lastPosition == QUEUE_SIZE)
				m_lastPosition = 0;
			::ReleaseMutex(m_mutex);
			//::LeaveCriticalSection(&queueSync);
		}
	}

private:
	static BOOL CALLBACK EnumWindowsProcWrapper(HWND hwnd, LPARAM lParam)
	{
		return ((CDataSenderMsg<T>*)lParam)->EnumWindowsProc(hwnd, lParam);
	}

	static DWORD WINAPI DoSendWrapper(LPVOID param)
	{
		return ((CDataSenderMsg<T>*)param)->DoSend(param);
	}

	DWORD WINAPI DoSend(LPVOID param)
	{
		//CWinMutex writeSyncMutex(m_mutexName, true);
		
		::SetEvent(m_readyEvent);
		
		while(!m_stop)
		{
			logger<<"DOSEND1\n";
			long prev;
			::WaitForSingleObject(m_mutex, INFINITE);
			//::EnterCriticalSection(&queueSync);
			if (::ReleaseSemaphore(m_semaphore, 1, &prev))
			{
				logger<<"DOSEND2\n";
				void* data = m_messageQueue[m_currentPosition];
				m_messageQueue[m_currentPosition] = NULL;
				//::LeaveCriticalSection(&queueSync);
				::ReleaseMutex(m_mutex);
				//if (writeSyncMutex.Lock(INFINITE))
				{
					DWORD id = ::GetCurrentProcessId();					
					
					COPYDATASTRUCT dataToSend;
					dataToSend.dwData = 111;
					dataToSend.cbData = sizeof(DWORD);
					dataToSend.lpData = &id;

					int attemp = 0;
					LRESULT res = 0;
					ULONG_PTR send_res;
					
					do{ 
						attemp++;
						res = ::SendMessageTimeout(m_hWnd, WM_COPYDATA, (WPARAM)m_hWnd, (LPARAM)(LPVOID)&dataToSend,
									SMTO_NORMAL, 1000, &send_res);	
					}while(attemp<3 && !m_stop && !res);
					
					delete data; 
					if (++m_currentPosition == QUEUE_SIZE)
						m_currentPosition = 0;
					//if (res)
						//logger<<"sent"<<'\n';
					//else
						//logger<<"failed"<<'\n';
					//writeSyncMutex.UnLock(); //m_writeSyncMutex->UnLock();
				}
			}else{
				::ReleaseMutex(m_mutex);
				//::LeaveCriticalSection(&queueSync);
			}
		}
		logger<<"EXIT"<<m_pid<<'\n';
		return 0;
	}

	BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
	{
		char name[MAX_SIZE];
		if (::GetWindowTextA(hwnd, name, MAX_SIZE))
		{
			if (strcmp(name, m_wndName)==0)
			{
				m_hWnd = hwnd;
			}
		}
		return (m_hWnd==NULL);
	}

	void FindTargetWindow()
	{
		::EnumWindows(EnumWindowsProcWrapper, reinterpret_cast<LPARAM>(this));
		if (m_hWnd==NULL)
			logger<<"window was not found..."<<'\n';
	}

	HWND m_hWnd;
	char m_wndName[MAX_SIZE];
	char m_mutexName[MAX_SIZE];
	void* m_messageQueue[QUEUE_SIZE];
	volatile int m_lastPosition;
	volatile int m_currentPosition;

	HANDLE m_semaphore;
	HANDLE m_mutex;
	HANDLE m_readyEvent;
	CRITICAL_SECTION queueSync;
	CWinMutex* m_writeSyncMutex;
	volatile BOOL m_stop;
	HANDLE m_sender;

	DWORD m_wndPid;
	DWORD m_pid;
};
