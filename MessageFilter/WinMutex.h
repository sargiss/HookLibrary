#pragma once
class CWinMutex
{
private: 
	volatile HANDLE m_hMutex;
public:
	CWinMutex(LPCSTR name, bool createNew);
	bool Lock(DWORD milliseconds);
	void UnLock();
};

