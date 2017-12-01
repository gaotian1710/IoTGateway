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
		uint8_t *buffer;
		ptrdiff_t wrOffset, rdOffset;
		uint32_t bcLength;
		CRITICAL_SECTION cs;
	} YTARingBuffer, *PYTARingBuffer;

	typedef const YTARingBuffer *PCYTARingBuffer;

	uint32_t YTARingBuffer_Init(PYTARingBuffer p, uint32_t length);
	uint32_t YTARingBuffer_Destroy(PYTARingBuffer p);
	PYTARingBuffer YTARingBuffer_New(uint32_t length);
	void YTARingBuffer_Delete(PYTARingBuffer *pp);

	/*!
	\brief Get bytes from the ring buffer.
	\param prb [in,out] ring buffer pointer
	\param bytesToPut [in] input data
	\param bcBytesToPut [in] byte count of the input data
	\return number of written bytes
	*/
	uint32_t YTARingBuffer_Put(
		PYTARingBuffer prb, const uint8_t* bytesToPut, uint32_t bcBytesToPut
	);

	uint32_t YTARingBuffer_Get(
		PYTARingBuffer prb, uint8_t* bytesToGet, uint32_t bcBytesToGet
	);
#pragma endregion YTARingBuffer_xxx

#pragma region YTASyncDetector_xxx
	typedef struct _YTASyncDetector {
		uint8_t* buffer;
		uint8_t* refferenceSequence;
		uint32_t bcLength;
		YTACallback callback;
		void* callbackParam;
		uint32_t callbackResult;
	} YTASyncDetector, *PYTASyncDetector;


	uint32_t YTASyncDetector_Init(
		PYTASyncDetector p,
		uint32_t syncSequenceLength,
		const uint8_t* syncSequence,
		YTACallback callback,
		void* callbackParam
	);

	uint32_t YTASyncDetector_Put(
		PYTASyncDetector p,
		uint8_t byte
	);
#pragma endregion YTASyncDetector_xxx

#if defined(__cplusplus)
}
#endif


