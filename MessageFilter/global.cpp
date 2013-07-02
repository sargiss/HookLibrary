#include "stdafx.h"
#include "global.h"
#include "HookInfo.h"
#include "MemoryMappedFile.h"
#include "resource.h"

HMODULE g_hModule;
CLogger logger(string("C:\\sniffer_log.txt"));
ParsedHookInfo g_hookInfo;

void LogLastError(const char* info)
{
	::LogLastError(::GetLastError(), info);
}

void LogLastError(DWORD code, const char* info)
{
	void* cstr;
	if (!code)
	{
		logger<<"Error code = 0\n";
		return;
	}
	DWORD bufferLen = ::FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		code,
		MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
		(LPTSTR) &cstr,
		0,
		NULL
	);
	if (!bufferLen)
	{
		logger<<"Buffer len = 0\n";
		return;
	}
	LPSTR msg = new char[bufferLen + 1];
	msg[bufferLen]='\0';
	wcstombs(msg, (wchar_t*)cstr, bufferLen);
	std::string result(msg);
	if (info)
		result = string(info) + result;
	delete msg;
	::LocalFree(cstr);
	logger<<"Error: "<<result<<'\n';
}

BOOL LoadResourceStringA(int ID, LPSTR buffer)
{
	int count = LoadStringA(g_hModule, ID, buffer, MAX_SIZE);
	buffer[count] = '\0';
	
	if (!count)
		LogLastError();
	
	return count!=0;
}

BOOL LoadResourceStringW(int ID, LPTSTR buffer)
{
	int count = LoadStringW(g_hModule, ID, buffer, MAX_SIZE);
	buffer[count] = '\0';
		
	if (!count)
		LogLastError();
	
	return count!=0;
}

void ReadSetting(ParsedHookInfo& hookInfo)
{
	CMemoryMappedFile settingFile;
	wchar_t fileName[MAX_SIZE];
	LoadResourceStringW(IDS_HOOKINFO, fileName);
	if (settingFile.Open(fileName))
	{
		FileHookInfo* setting = (FileHookInfo*)settingFile.GetPointer();
		hookInfo.Init(*setting);
	}
}