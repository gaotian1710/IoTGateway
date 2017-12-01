#include "stdafx.h"
#include "CppUnitTest.h"
#include "TestUtils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace APIsTests
{
	
	TEST_CLASS(UT_Fsm)
	{
		enum StateId {
			StateId_Default,
			StateId_1,
			StateId_2,
			StateId_3
		};

		static const YTAState States[];
		
		static YTASHResult SH1(PYTAFsm fsm, PYTAFsmMsg msg);
		static YTASHResult SH2(PYTAFsm fsm, PYTAFsmMsg msg);
		static YTASHResult SH3(PYTAFsm fsm, PYTAFsmMsg msg);
		static void AHEnter1(PYTAFsm fsm);
		static void AHEnter2(PYTAFsm fsm);
		static void AHExit3(PYTAFsm fsm);

		TEST_METHOD(UT_Transit_1_2_1_3_1_Exit)
		{
			YTAFsm fsm = { NULL, NULL, &States[0] };
			YTAFsmMsg msgs[] = {
				{ NULL, 2 },
				{ NULL, 2 },
				{ NULL, 3 },
				{ NULL, 3 },
				{ NULL, 1 }
			};

			YTAFsm_InitialTransit(&fsm, &States[1]);
			Assert::AreEqual((const void*)fsm.state, (const void*)&States[1], L"initial state");
			for (int iMsg = 0; iMsg < sizeof(msgs) / sizeof(msgs[0]); iMsg++)
			{
				YTASHResult result = YTAFsm_Transit(&fsm, &msgs[iMsg]);
			}
			Assert::AreEqual((const void*)fsm.state, (const void*)&States[0], L"final state");
		}
	};

	const YTAState UT_Fsm::States[] = {
		{ YTASHNone, YTAAHNone, YTAAHNone },
		{ UT_Fsm::SH1, UT_Fsm::AHEnter1, YTAAHNone },
		{ UT_Fsm::SH2, UT_Fsm::AHEnter2, YTAAHNone },
		{ UT_Fsm::SH3, YTAAHNone, UT_Fsm::AHExit3 }
	};

	YTASHResult UT_Fsm::SH1(PYTAFsm fsm, PYTAFsmMsg msg)
	{
		fsm->msg = msg;
		YTASHResult result = YTASHResult_Ignored;
		switch (msg->msg)
		{
		case 1:
			fsm->state = &States[0];
			result = YTASHResult_Transited;
			TestLog(L"Goodbye");
			break;
		case 2: // S1 --> S2
			fsm->state = &States[2];
			result = YTASHResult_Transited;
			TestLog(L"Hello_S1-2");
			break;
		case 3:
			fsm->state = &States[3];
			result = YTASHResult_Transited;
			TestLog(L"Hello_S1-3");
			break;
		default:
			break;
		}
		return result;
	}

	YTASHResult UT_Fsm::SH2(PYTAFsm fsm, PYTAFsmMsg msg)
	{
		fsm->msg = msg;
		YTASHResult result = YTASHResult_Ignored;
		switch (msg->msg)
		{
		case 2:
			fsm->state = &States[1];
			result = YTASHResult_Transited;
			TestLog(L"Hello_S2-1");
			break;
		default:
			break;
		}
		return result;
	}

	YTASHResult UT_Fsm::SH3(PYTAFsm fsm, PYTAFsmMsg msg)
	{
		fsm->msg = msg;
		YTASHResult result = YTASHResult_Ignored;
		switch (msg->msg)
		{
		case 1:
			result = YTASHResult_Transited;
			TestLog(L"Good bye");
			fsm->state = &States[0];
			break;
		default:
			break;
		}
		return YTASHResult();
	}

	void UT_Fsm::AHEnter1(PYTAFsm fsm)
	{
		TestLog(L"S1 entered.");
	}

	void UT_Fsm::AHEnter2(PYTAFsm fsm)
	{
		TestLog(L"S2 entered.");
	}

	void UT_Fsm::AHExit3(PYTAFsm fsm)
	{
		TestLog(L"S3 exiting.");
	}
}
