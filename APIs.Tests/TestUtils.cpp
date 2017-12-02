#include "stdafx.h"
#include "TestUtils.h"

WCHAR TestUtils::logBuffer[TestUtils::logBufferLength];

TestUtils::TestUtils()
{
}


TestUtils::~TestUtils()
{
}

/*
$(SolutionDir)TestData -> $(OutDir)\datadir.txt
*/
PYTAWTextBuffer TestUtils::TestDataDir()
{
	static YTAByteBuffer byteBuffer = { 0 };
	static YTAWTextBuffer wtextBuffer = { 0 };
	YTAWTextBuffer_Init(&wtextBuffer, MAX_PATH);
	GetCurrentDirectory(wtextBuffer.ccLength, wtextBuffer.buffer);
	PathAppend(wtextBuffer.buffer, L"testdatadir.txt");
	if (!PathFileExists(wtextBuffer.buffer)) return nullptr;

	uint32_t err = YTATextRW_ReadBytes(&byteBuffer, wtextBuffer.buffer);
	if (err != ERROR_SUCCESS) return nullptr;
	while (byteBuffer.buffer[byteBuffer.bcFilled - 1] <= ' ')
	{
		byteBuffer.bcFilled--;
	}
	YTAWTextBuffer_Init(&wtextBuffer, byteBuffer.bcFilled + 1);
	int i = MultiByteToWideChar(CP_ACP, 0, (LPCCH)byteBuffer.buffer, byteBuffer.bcFilled, wtextBuffer.buffer, wtextBuffer.ccLength);
	if (i == 0)
	{
		return nullptr;
	}
	wtextBuffer.ccFilled = (size_t)i;
	wtextBuffer.buffer[wtextBuffer.ccFilled] = L'\0';
	return &wtextBuffer;
}
