#include "stdafx.h"
#include "CppUnitTest.h"
#include "TestUtils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace APIsTests
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(UT_YTAWTextBuffer_Init)
		{
			YTAWTextBuffer buffer = { 0 };
			uint32_t err = YTAWTextBuffer_Init(&buffer, MAX_PATH);
			Assert::AreEqual((uint32_t)ERROR_SUCCESS, err);
			Assert::AreEqual(buffer.ccFilled, (uint32_t)0);
			Assert::AreEqual(buffer.ccLength, (uint32_t)MAX_PATH);
			err = YTAWTextBuffer_Destroy(&buffer);
			Assert::AreEqual((uint32_t)ERROR_SUCCESS, err);
		}


	};
}