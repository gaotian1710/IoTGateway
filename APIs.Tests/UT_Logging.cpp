#include "stdafx.h"
#include "CppUnitTest.h"
#include "TestUtils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace APIsTests
{
	TEST_CLASS(UT_Logging)
	{
	public:
		TEST_METHOD(Logging_BeginEnd)
		{
			Logging_Begin(L"test.log", Logging_LevelDebug);
			Logging_Debug(L"%hs,%d", "Hello",1);
			YTABaseDefs_SetLocation(__FILE__, __LINE__);
			Logging_Error(L"%d,%u",-10,10);
			Logging_End();
		}
	};
}
