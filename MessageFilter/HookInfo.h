#include "stdafx.h"
#include <set>

#pragma once

using namespace std;

struct FileHookInfo
{
	int processId;
	int mesageCount;
};

struct ParsedHookInfo
{
	void Init(const FileHookInfo& hookInfo)
	{
		processId = hookInfo.processId;
		
		for(int i=1; i<=hookInfo.mesageCount; i++)
		{
			int msg = *((&hookInfo.mesageCount) + i);
			messages.insert(msg);
		}
	}

	int processId;
	set<int> messages;
};