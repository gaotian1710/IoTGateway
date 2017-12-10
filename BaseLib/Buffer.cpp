#include "stdafx.h"
#include "BaseDefs.h"
#include "Buffer.h"


#pragma region YTAWTextBuffer_xxx
uint32_t YTAWTextBuffer_Init(PYTAWTextBuffer p, uint32_t length)
{
	uint32_t err = ERROR_SUCCESS;
	do {
		if (p->buffer) free(p->buffer);
		p->ccLength = p->ccFilled = 0;
		p->buffer = (wchar_t*)malloc(sizeof(wchar_t) * length);
		if (p->buffer == NULL)
		{
			err = ERROR_NOT_ENOUGH_MEMORY;
			break;
		}
		p->ccLength = length;
	} while (0);
	return err;
}

uint32_t YTAWTextBuffer_Destroy(PYTAWTextBuffer p)
{
	uint32_t err = ERROR_SUCCESS;
	if (p == nullptr) return err;
	if (p->buffer == nullptr) return err;
	free((void*)(p->buffer));
	p->ccFilled = p->ccLength = 0;
	p->buffer = nullptr;
	return err;
}

PYTAWTextBuffer YTAWTextBuffer_New(uint32_t length)
{
	PYTAWTextBuffer p = (YTAWTextBuffer*)malloc(sizeof(YTAWTextBuffer) + sizeof(wchar_t) * length);
	if (p)
	{
		p->buffer = (wchar_t*)(p + 1);
		p->ccLength = length;
		p->ccFilled = 0;
	}
	return p;
}

void YTAWTextBuffer_Delete(PYTAWTextBuffer * pp)
{
	if (pp == NULL) return;
	if (*pp != NULL)
	{
		free((void*)(*pp));
		*pp = NULL;
	}
}

#pragma endregion YTAWTextBuffer_xxx

#pragma region YTAByteBuffer_xxx
uint32_t YTAByteBuffer_Init(PYTAByteBuffer p, uint32_t length)
{
	uint32_t err = ERROR_SUCCESS;
	do {
		if (p->buffer) free((void*)(p->buffer));
		p->bcLength = p->bcFilled = 0;
		p->buffer = (uint8_t*)malloc(length);
		if (p->buffer == NULL)
		{
			err = ERROR_NOT_ENOUGH_MEMORY;
			break;
		}
		p->bcLength = length;
	} while (0);
	return err;
}

uint32_t YTAByteBuffer_Destroy(PYTAByteBuffer p)
{
	uint32_t err = ERROR_SUCCESS;
	do {
		if (p == nullptr) break;
		if (p->buffer == nullptr) break;
		free(p->buffer);
		p->buffer = nullptr;
		p->bcFilled = p->bcLength = 0;
	} while (0);
	return err;
}

PYTAByteBuffer YTAByteBuffer_New(uint32_t length)
{
	PYTAByteBuffer p = (PYTAByteBuffer)malloc(sizeof(YTAByteBuffer) + length);
	if (p)
	{
		p->buffer = (uint8_t*)(p + 1);
		p->bcLength = length;
		p->bcFilled = 0;
	}
	return p;
}

void YTAByteBuffer_Delete(PYTAByteBuffer * pp)
{
	if (pp == NULL) return;
	if (*pp)
	{
		free(*pp);
		*pp = NULL;
	}
}

#pragma endregion YTAByteBuffer_xxx

#pragma region YTARingBuffer_xxx
uint32_t YTARingBuffer_Init(PYTARingBuffer p, uint8_t length)
{
	if (p->buffer) YTARingBuffer_Destroy(p);
	p->buffer = (uint8_t*)malloc(length);
	p->length = length;
	return ERROR_SUCCESS;
}

uint32_t YTARingBuffer_Destroy(PYTARingBuffer p)
{
	if (p->buffer)
	{
		free(p->buffer); p->buffer = NULL;
	}
	p->length = p->writeIndex = p->readIndex = 0;
	return ERROR_SUCCESS;
}

uint8_t YTARingBuffer_NextWriteIndex(PYTARingBuffer p)
{
	return (p->writeIndex + 1) % p->length;
}

uint8_t YTARingBuffer_NextReadIndex(PYTARingBuffer p)
{
	return (p->readIndex + 1) % p->length;
}

uint32_t YTARingBuffer_PutByte(PYTARingBuffer p, const uint8_t* byteData)
{
	uint32_t err = ERROR_SUCCESS;
	uint8_t nextWrIndex = YTARingBuffer_NextWriteIndex(p);
	do {
		if (nextWrIndex == p->readIndex)
		{
			err = ERROR_INSUFFICIENT_BUFFER;
			break;
		}
		*(p->buffer + p->writeIndex) = *byteData;
		p->writeIndex = nextWrIndex;
	} while (false);
	return err;
}

uint32_t YTARingBuffer_GetByte(PYTARingBuffer p, uint8_t * byteData)
{
	uint32_t err = ERROR_SUCCESS;
	do {
		if (p->readIndex == p->writeIndex)
		{
			err = ERROR_END_OF_MEDIA;
			break;
		}
		*byteData = *(p->buffer + p->readIndex);
		p->readIndex = YTARingBuffer_NextReadIndex(p);
	} while (false);
	return err;
}

#pragma endregion YTARingBuffer_xxx

