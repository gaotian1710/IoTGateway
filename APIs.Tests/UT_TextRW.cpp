#include "stdafx.h"
#include "CppUnitTest.h"
#include "TestUtils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace APIsTests
{
	TEST_CLASS(CRC16)
	{
	public:

		TEST_METHOD(UT_TextRW_Ascii)
		{
			PYTAWTextBuffer testDataDir = TestUtils::TestDataDir();
			YTAWTextBuffer testDataPath = { 0 }; YTAWTextBuffer_Init(&testDataPath, MAX_PATH);
			wcscpy_s(testDataPath.buffer, MAX_PATH, testDataDir->buffer);
			PathAppend(testDataPath.buffer, L"TextRW\\texta0.txt");
			YTAByteBuffer testBuffer = { 0 };
			uint32_t err = YTATextRW_ReadBytes(&testBuffer, testDataPath.buffer);
			Assert::AreEqual((uint32_t)ERROR_SUCCESS, err, L"YTATextRW_ReadBytes() should succeed.");
			wcscpy_s(testDataPath.buffer, MAX_PATH, testDataDir->buffer);
			PathAppend(testDataPath.buffer, L"TextRW\\texta1.txt");
			if (PathFileExists(testDataPath.buffer))
			{
				Assert::AreEqual(TRUE, DeleteFile(testDataPath.buffer), L"DeleteFile(\"texta1.txt\") failed.");
			}
			err = YTATextRW_WriteBytes(&testBuffer, testDataPath.buffer);
			Assert::AreEqual((uint32_t)ERROR_SUCCESS, err, L"YTATextRW_WriteBytes() should succeed.");
		}
	};
}