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
	free((void*)(p->buffer));
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
		free(p->buffer);
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
uint32_t YTARingBuffer_Init(PYTARingBuffer p, uint32_t length)
{
	if (p->buffer)
	{
		free(p->buffer);
		p->rdOffset = p->wrOffset = 0;
		DeleteCriticalSection(&(p->cs));
	}
	p->buffer = (uint8_t*)malloc(length);
	if (p->buffer)
	{
		p->bcLength = length;
		InitializeCriticalSection(&(p->cs));
	}
	return (p->buffer != NULL) ? ERROR_SUCCESS : ERROR_NOT_ENOUGH_MEMORY;
}

uint32_t YTARingBuffer_Destroy(PYTARingBuffer p)
{
	free(p->buffer);
	p->buffer = NULL;
	p->bcLength = 0;
	p->rdOffset = p->wrOffset = 0;
	DeleteCriticalSection(&(p->cs));
	return ERROR_SUCCESS;
}

PYTARingBuffer YTARingBuffer_New(uint32_t length)
{
	PYTARingBuffer p = (PYTARingBuffer)malloc(sizeof(YTARingBuffer) + length);
	if (p)
	{
		p->buffer = (uint8_t*)(p + 1);
		p->rdOffset = p->wrOffset = 0;
		p->bcLength = length;
		InitializeCriticalSection(&(p->cs));
	}
	return p;
}

void YTARingBuffer_Delete(PYTARingBuffer * pp)
{
	if (*pp)
	{
		DeleteCriticalSection(&((*pp)->cs));
		free(*pp);
		*pp = NULL;
	}
}

ptrdiff_t _YTARingBuffer_NextWrOffset(PYTARingBuffer prg)
{
	return (prg->wrOffset + 1) % (prg->bcLength);
}
uint32_t _YTARingBuffer_PutByte(PYTARingBuffer prb, const uint8_t *pbyte)
{
	ptrdiff_t nextWrOffset = _YTARingBuffer_NextWrOffset(prb);
	if (nextWrOffset == prb->rdOffset) return 0;
	else
	{
		*(prb->buffer + prb->wrOffset) = *pbyte;
		prb->wrOffset = nextWrOffset;
	}
	return 1;
}

uint32_t YTARingBuffer_Put(PYTARingBuffer prb, const uint8_t* bytesToPut, uint32_t bcBytesToPut)
{
	uint32_t putCount = 0;
	if (bcBytesToPut == 0) return putCount;
	EnterCriticalSection(&(prb->cs));
	while (_YTARingBuffer_PutByte(prb, bytesToPut))
	{
		bytesToPut++;
		putCount++;
		if (--bcBytesToPut == 0) break;
	}
	LeaveCriticalSection(&(prb->cs));
	return putCount;
}

ptrdiff_t _YTARingBuffer_NextRdOffset(PYTARingBuffer prb)
{
	return (prb->rdOffset + 1) % (prb->bcLength);
}

uint32_t _YTARingBuffer_GetByte(PYTARingBuffer prb, uint8_t* pbyte)
{
	if (prb->rdOffset == prb->wrOffset) return 0;
	else
	{
		*pbyte = *(prb->buffer + prb->rdOffset);
		prb->rdOffset = _YTARingBuffer_NextRdOffset(prb);
		return 1;
	}
}
uint32_t YTARingBuffer_Get(PYTARingBuffer prb, uint8_t * bytesToGet, uint32_t bcBytesToGet)
{
	uint32_t getCount = 0;
	if (bcBytesToGet == 0) return getCount;
	EnterCriticalSection(&(prb->cs));
	while (_YTARingBuffer_GetByte(prb, bytesToGet))
	{
		bytesToGet++;
		getCount++;
		if (--bcBytesToGet == 0) break;
	}
	LeaveCriticalSection(&(prb->cs));
	return getCount;
}

#pragma endregion YTARingBuffer_xxx

#pragma region YTASyncDetector_xxx

uint32_t YTASyncDetector_Init(
	PYTASyncDetector p, uint32_t syncSequenceLength, const uint8_t * syncSequence,
	YTACallback callback, void * callbackParam)
{
	uint32_t err = ERROR_SUCCESS;
	do {
		if (p->buffer)
		{
			free(p->buffer);
			p->bcLength = 0;
		}
		p->buffer = (uint8_t*)malloc(syncSequenceLength * 2);
		if (p->buffer == NULL)
		{
			err = ERROR_NOT_ENOUGH_MEMORY;
			break;
		}
		p->refferenceSequence = p->buffer + syncSequenceLength;
		if (syncSequence)
		{
			strcpy_s((char*)(p->refferenceSequence), p->bcLength, (const char*)syncSequence);
		}
		p->callback = callback;
		p->callbackParam = callbackParam;
	} while (0);
	return err;
}

void _YTASyncDetector_ShiftIn(PYTASyncDetector p, uint8_t byte)
{
	uint8_t *pDst = p->buffer;
	const uint8_t *pSrc = (const uint8_t*)(pDst + 1);

	for (uint32_t i = 0; i < p->bcLength - 1; i++)
	{
		*pDst++ = *pSrc++;
	}
	*pDst = byte;
}
uint32_t YTASyncDetector_Put(PYTASyncDetector p, uint8_t byte)
{
	uint32_t detect = 0;
	_YTASyncDetector_ShiftIn(p, byte);
	if (memcmp(p->buffer, p->refferenceSequence, p->bcLength) == 0)
	{
		detect = 1;
		p->callbackResult = p->callback(p->callbackParam);
	}
	return detect;
}

#pragma endregion YTASyncDetector_xxx

