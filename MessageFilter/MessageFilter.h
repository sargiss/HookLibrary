#include "stdafx.h"
#include "WinUser.h"

extern "C" __declspec(dllexport) LRESULT CALLBACK CallWindProc(_In_  int nCode, _In_  WPARAM wParam, _In_  LPARAM lParam);
extern "C" __declspec(dllexport) LRESULT CALLBACK GetMsgProc(_In_  int nCode, _In_  WPARAM wParam, _In_  LPARAM lParam);

template <class T = int>
struct DataToSend
{
	int message;
	int hook_type;
	HWND hWnd;
	T data;
};
