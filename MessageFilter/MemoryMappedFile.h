#include "stdafx.h"

class CMemoryMappedFile
{
public:
	CMemoryMappedFile():m_hMappedFile(NULL), m_pView(NULL)
	{ }

	~CMemoryMappedFile()
	{
		if (m_hMappedFile)
			Close();
	}

	bool Open(LPCTSTR fileName)
	{
		m_hMappedFile = ::OpenFileMapping(FILE_MAP_READ, false, fileName);
		
		if (m_hMappedFile)
		{
			m_pView = ::MapViewOfFile(m_hMappedFile, FILE_MAP_READ, 0, 0, 0);
			return m_pView != NULL;
		}
		LogLastError("Opening memory mapped file: ");
		return 0;
	}

	void* GetPointer()
	{
		return m_pView;
	}

	void Close()
	{
		::UnmapViewOfFile(m_pView);
		::CloseHandle(m_hMappedFile);
		m_hMappedFile = NULL;
	}

private:
	HANDLE m_hMappedFile;
	void* m_pView;
};