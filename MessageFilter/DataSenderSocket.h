#include "winsock2.h"
#include <ws2tcpip.h>
#include <stdio.h>
#include "WinMutex.h"
#include "DataSender.h"

#pragma once

template <class T>
/*class CDataSenderSocket: public CDataSender<T>
{
public:
	CDataSenderSocket();
	~CDataSenderSocket();
	void Send(const T& data);
	void Release();
private:
	void Connect();
private:
	SOCKET m_socket;
	CWinMutex m_writeSyncMutex;
};

*/