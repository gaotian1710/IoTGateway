// TestApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int wmain()
{
	Logging_Begin(L"test.log", Logging_LevelDebug);
	Logging_Debug(L"%hs,%d,%ws", __FILE__, __LINE__, L"こんにちは");
	Logging_End();
    return 0;
}

