#include "StdAfx.h"
#include "WinMutex.h"
#include <WinBase.h>
#include <iostream>
#include <sstream>
#include "global.h"

CWinMutex::CWinMutex(LPCSTR name, bool createNew)
{
	if (createNew)
	{
		m_hMutex = ::CreateMutexA(NULL, false, name);
	}
	else
	{
		m_hMutex = ::OpenMutexA(MUTEX_ALL_ACCESS, true, name);
	}
	
	if (m_hMutex == NULL)
	{
		std::ostringstream stream;
		stream<<"\nCan not create or open mutex with name: "<<name<<" Code: "<<::GetLastError();
		std::string text = stream.str();
		logger<<text.c_str();
		throw std::runtime_error(text);
	};
}

bool CWinMutex::Lock(DWORD milliseconds)
{
	DWORD wait_result = ::WaitForSingleObject(m_hMutex, milliseconds);
	logger<<"wr="<<wait_result<<" "<<::GetCurrentThreadId()<<'\n';
	return (wait_result == WAIT_OBJECT_0);
}

void CWinMutex::UnLock()
{
	::ReleaseMutex(m_hMutex);
}
