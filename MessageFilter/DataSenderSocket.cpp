#include "StdAfx.h"
/*#include "DataSenderSocket.h"
#include "WinMutex.h"

/*
#pragma comment(lib, "ws2_32.lib")

template < class T>
CDataSenderSocket<T>::CDataSenderSocket():m_writeSyncMutex("data_sender_sync", true)
{
	m_socket = INVALID_SOCKET;

	WSADATA wsaData;
    int err;

    err = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (err != 0) {
        printf("WSAStartup failed with error: %d\n", err);
    }
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
        printf("Could not find a usable version of Winsock.dll\n");
        WSACleanup();
    }
    else
        printf("The Winsock 2.2 dll was found okay\n");
}

template <class T>
CDataSenderSocket<T>::~CDataSenderSocket()
{
	Release();
	WSACleanup();
}

template <class T>
inline void CDataSenderSocket<T>::Send(const T& data)
{
	if (m_socket == INVALID_SOCKET)
		Connect();
	sockaddr_in addr_inf;
	addr_inf.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addr_inf.sin_port = htons(2666);
	addr_inf.sin_family = AF_INET;
	int size = sizeof(T);
	if (m_writeSyncMutex.Lock(1000))
		sendto(m_socket, (char*)&data, size, 0, (SOCKADDR*)&addr_inf, sizeof(addr_inf));
	m_writeSyncMutex.UnLock();
}

template <class T>
void CDataSenderSocket<T>::Connect()
{
	m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (m_socket == INVALID_SOCKET) 
        wprintf(L"socket function failed with error = %d\n", WSAGetLastError() );
    else {
        wprintf(L"socket function succeeded\n");
	}
}

template <class T>
void CDataSenderSocket<T>::Release()
{
	closesocket(m_socket);
	m_socket = INVALID_SOCKET;
}
*/