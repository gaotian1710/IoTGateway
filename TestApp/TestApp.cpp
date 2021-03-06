// TestApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int wmain()
{
	uint32_t err = ERROR_SUCCESS;
	do {
		YTAByteBuffer bb = { 0 };
		err = YTATextRW_ReadBytes(&bb, L"..\\TestData\\TextRW\\texta0.txt");
		wprintf(L"%hs", bb.buffer);
		err = YTATextRW_WriteBytes(&bb, L"..\\TestData\\TextRW\\texta1.txt");

		YTAWTextBuffer wtb = { 0 };
		err = YTATextRW_ReadWText(&wtb, L"..\\TestData\\TextRW\\textu16-0.txt");
		wprintf(L"%ws", wtb.buffer);
		wprintf(L"wtb.ccFilled = %d, wtb.ccLength = %d\n", wtb.ccFilled, wtb.ccLength);
		err = YTATextRW_WriteWText(&wtb, L"..\\TestData\\TextRW\\textu16-1.txt");
	} while (false);
    return err;
}

