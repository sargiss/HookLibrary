#include "stdafx.h"
#include "Logger.h"
#include "HookInfo.h"

#pragma once

#define MAX_SIZE 256

extern HMODULE g_hModule;
extern CLogger logger;
extern ParsedHookInfo g_hookInfo;

void LogLastError(const char* info = NULL);
void LogLastError(DWORD errorCode, const char* info = NULL);
BOOL LoadResourceStringA(int ID, LPSTR buffer);
BOOL LoadResourceStringW(int ID, LPTSTR buffer);
void ReadSetting(ParsedHookInfo& hookInfo);