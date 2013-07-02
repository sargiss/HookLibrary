#include "winsock2.h"
#include <ws2tcpip.h>
#include <stdio.h>

#pragma once

template <class T>
class CDataSender
{
public:
	virtual ~CDataSender(){};
	virtual void Send(const T& data) = 0;
};

