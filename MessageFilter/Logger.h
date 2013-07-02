#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <fstream>

#pragma once

using namespace std;

class CLogger 
{
public: 
	CLogger(string fileName): m_fileName(fileName)
	{ 
	}

	~CLogger()
	{
		if (m_logFile.is_open())
			m_logFile.close();
	}

	template <class T>
	CLogger& operator<<(const T& object)
	{
		Open();

		m_logFile<<object;
		m_logFile.flush();
		//*/
		return *this;
	}

	void Open()
	{
		if (!m_logFile.is_open())
			m_logFile.open(m_fileName, ios_base::app);
	}
private:
	string m_fileName;
	ofstream m_logFile;
};