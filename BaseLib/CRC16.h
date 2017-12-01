#pragma once
#if defined(__cplusplus)
// Please see docs\CRC16

extern "C" {
#endif
	typedef union {
		uint16_t word;
		uint8_t  bytePair[2];
	} YTASwappableBytes;

	typedef struct _YTACRC16Data {
		uint16_t
			genPoly; // generator polynomial
		YTASwappableBytes
			sr; // shift register
	} YTACRC16Data, *PYTACRC16Data;

	typedef void(*CRC16PutByte)(PYTACRC16Data p, const uint8_t* pbyte);

	typedef struct _YTACRC16 {
		CRC16PutByte pbfn;
		YTACRC16Data data;
	} YTACRC16, *PYTACRC16;

	/*!
	\brief raw access to shift register without swapping byte pair.
	*/
	void YTACRC16_PutSR(PYTACRC16 p, uint16_t rawWord);

	/*!
	\brief raw access to shift register without swapping byte pair.
	*/
	void YTACRC16_GetSR(PYTACRC16 p, uint16_t* rawWord);

	/*!
	\brief get 16 bit CRC, swapping byte pair.
	\param crc [out] two byte array, crc[0]: first byte/MSB, crc[1]: second byte/LSB
	*/
	void YTACRC16_GetCRC(PYTACRC16 p, uint8_t* crc);

	/*!
	\brief put a data byte
	*/
	void YTACRC16_PutDataByte(
		PYTACRC16 p, const uint8_t* pbyte
	);

	/*!
	\brief put an array of bytes
	*/
	void YTACRC16_PutDataBytes(
		PYTACRC16 p, uint32_t length, const uint8_t* pbytes
	);

	// put byte MSB first
	void YTACRC16_PutByteMF(PYTACRC16Data p, const uint8_t* pbyte);
	void YTACRC16_PutByteLF(PYTACRC16Data p, const uint8_t* pbyte);

	void YTACRC16_InitCCITTCRC16(PYTACRC16 p, uint16_t seed);
	void YTACRC16_InitCCITTCRC16_LSB1st(PYTACRC16 p, uint16_t seed);
#if defined(__cplusplus)
}
#endif
