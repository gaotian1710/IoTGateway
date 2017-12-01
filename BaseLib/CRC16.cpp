#include "stdafx.h"
#include "CRC16.h"

void YTACRC16_PutSeed(PYTACRC16 p, uint16_t rawWord)
{
	p->data.sr.word = rawWord;
}

void YTACRC16_GetSR(PYTACRC16 p, uint16_t * rawWord)
{
	*rawWord = p->data.sr.word;
}

void YTACRC16_GetCRC(PYTACRC16 p, uint8_t * crc)
{
	crc[0] = p->data.sr.bytePair[1];
	crc[1] = p->data.sr.bytePair[0];
}

void YTACRC16_PutDataByte(PYTACRC16 p, const uint8_t* pbyte)
{
	p->pbfn(&(p->data), pbyte);
}

void YTACRC16_PutDataBytes(PYTACRC16 p, uint32_t length, const uint8_t* pbytes)
{
	for (uint32_t i = 0; i < length; i++)
	{
		YTACRC16_PutDataByte(p, pbytes++);
	}
}


void YTACRC16_PutByteMF(PYTACRC16Data p, const uint8_t* pbyte)
{
	uint16_t inByte = (uint16_t)(*pbyte) << 8;
	for (int i = 0; i < 8; i++)
	{
		if ((inByte ^ p->sr.word) >> 15)
			p->sr.word = (p->sr.word << 1) ^ (p->genPoly);
		else
			p->sr.word <<= 1;
		inByte <<= 1;
	}
}

void YTACRC16_PutByteLF(PYTACRC16Data p, const uint8_t * pbyte)
{
	uint16_t inByte = (uint16_t)(*pbyte);
	for (int i = 0; i < 8; i++)
	{
		if ((inByte ^ p->sr.word) & 1)
			p->sr.word = (p->sr.word >> 1) ^ (p->genPoly);
		else
			p->sr.word >>= 1;
		inByte >>= 1;
	}
}

void YTACRC16_InitCCITTCRC16(PYTACRC16 p, uint16_t seed)
{
	p->data.sr.word = seed;
	p->data.genPoly = 0x1021;
	p->pbfn = YTACRC16_PutByteMF;
}

void YTACRC16_InitCCITTCRC16_LSB1st(PYTACRC16 p, uint16_t seed)
{
	p->data.sr.word = seed;
	p->data.genPoly = 0x8408; // Bit order reversed bit pattern of CCITT CRC16
	p->pbfn = YTACRC16_PutByteLF;
}
