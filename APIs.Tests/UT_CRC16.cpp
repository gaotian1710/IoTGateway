#include "stdafx.h"
#include "CppUnitTest.h"
#include "TestUtils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace APIsTests
{
	TEST_CLASS(CRC16)
	{
	public:

		TEST_METHOD(CRC16_BasicLF)
		{
			// sample data same as MSP432 sample code
			const uint8_t myData[9] = { 0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39 };
			// my own sample data
			const char* data1 = "Hello CRC16 --- ][>,<()ABC0123\n\r";
			CRCCheckMF(L"myData[9]", __ARRAYSIZE__(myData), myData);
			CRCCheckLF(L"myData[9]", __ARRAYSIZE__(myData), myData);
		}

		void CRCCheckMF(const wchar_t* title, size_t length, const uint8_t* data)
		{
			YTACRC16 crcEngine;
			union {
				uint8_t crc[2];
				uint16_t swappedCRC;
			} crc;
			uint16_t checkSum, rawCRC;

			YTACRC16_InitCCITTCRC16(&crcEngine, 0xffff);

			YTACRC16_PutDataBytes(&crcEngine, (uint32_t)length, data);
			YTACRC16_GetCRC(&crcEngine, crc.crc);
			YTACRC16_GetSR(&crcEngine, &rawCRC);
			YTACRC16_PutDataBytes(&crcEngine, 2, crc.crc);
			YTACRC16_GetSR(&crcEngine, &checkSum);
			TestLog(L"%ws", title);
			TestLog(L"Raw CRC=0x%04x(%05u), CRC=0x%04x(%05u)", rawCRC, rawCRC, crc.swappedCRC, crc.swappedCRC);
			TestLog(L"Final SR=0x%04x(%05u)", checkSum, checkSum);
		}

		void CRCCheckLF(const wchar_t* title, size_t length, const uint8_t* data)
		{
			YTACRC16 crcEngine;
			union {
				uint8_t crc[2];
				uint16_t swappedCRC;
			} crc;
			uint16_t checkSum, rawCRC;

			YTACRC16_InitCCITTCRC16_LSB1st(&crcEngine, 0xffff);

			YTACRC16_PutDataBytes(&crcEngine, (uint32_t)length, data);
			YTACRC16_GetSR(&crcEngine, &(crc.swappedCRC));
			YTACRC16_GetSR(&crcEngine, &rawCRC);
			YTACRC16_PutDataBytes(&crcEngine, 2, crc.crc);
			YTACRC16_GetSR(&crcEngine, &checkSum);
			TestLog(L"%ws", title);
			TestLog(L"Raw CRC=0x%04x(%05u), CRC=0x%04x(%05u)", rawCRC, rawCRC, crc.swappedCRC, crc.swappedCRC);
			TestLog(L"Final SR=0x%04x(%05u)", checkSum, checkSum);
		}

	};
}
