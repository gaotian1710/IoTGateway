#pragma once
#if defined(__cplusplus)
extern "C" {
#endif
#pragma region YTAWTextBuffer_xxx
	typedef struct _YTAWTextBuffer {
		wchar_t* buffer;
		uint32_t ccLength, ccFilled;
	} YTAWTextBuffer, *PYTAWTextBuffer;

	typedef const YTAWTextBuffer *PCYTAWTextBuffer;

	uint32_t YTAWTextBuffer_Init(PYTAWTextBuffer p, uint32_t length);
	uint32_t YTAWTextBuffer_Destroy(PYTAWTextBuffer p);
	PYTAWTextBuffer YTAWTextBuffer_New(uint32_t length);
	void YTAWTextBuffer_Delete(PYTAWTextBuffer *pp);

#define YTAWTextBuffer_buffer_length_(p) p->buffer, p->ccLength, p->ccFilled

#pragma endregion YTAWTextBuffer_xxx

#pragma region YTAByteBuffer_xxx
	typedef struct _YTAByteBuffer {
		uint8_t* buffer;
		uint32_t bcLength, bcFilled;
	} YTAByteBuffer, *PYTAByteBuffer;

	typedef const YTAByteBuffer *PCYTAByteBuffer;

	uint32_t YTAByteBuffer_Init(PYTAByteBuffer p, uint32_t length);
	uint32_t YTAByteBuffer_Destroy(PYTAByteBuffer p);
	PYTAByteBuffer YTAByteBuffer_New(uint32_t length);
	void YTAByteBuffer_Delete(PYTAByteBuffer *pp);
#pragma endregion YTAByteBuffer_xxx

#pragma region YTARingBuffer_xxx
	typedef struct _YTARingBuffer {
		uint8_t* buffer;
		uint8_t  length;
		uint8_t  writeIndex;
		uint8_t  readIndex;
	} YTARingBuffer, *PYTARingBuffer;

	/*!
	\brief initialize ring buffers
	\param p [in,out] buffer pointer
	\param length [in] buffer length
	*/
	uint32_t YTARingBuffer_Init(PYTARingBuffer p, uint8_t length);

	uint32_t YTARingBuffer_Destroy(PYTARingBuffer p);

	uint32_t YTARingBuffer_PutByte(PYTARingBuffer p, const uint8_t* byteData);

	uint32_t YTARingBuffer_GetByte(PYTARingBuffer p, uint8_t* byteData);

#pragma endregion YTARingBuffer_xxx

#if defined(__cplusplus)
}
#endif


