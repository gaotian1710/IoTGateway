#pragma once
/*! \file TextRW.h
\brief Text file R/W methods
*/
#if defined(__cplusplus)
extern "C" {
#endif
	/*!
	\brief read bytes from a file. buffer->buffer is resize/allocated
		automatically.
	\param buffer [out] byte buffer
	\param filePath [in] path of the file to read.
	\return win32 error code like ERROR_FILE_NOT_FOUND,...
	*/
	uint32_t YTATextRW_ReadBytes(PYTAByteBuffer buffer, const wchar_t* filePath);

	/*!
	\brief write bytes to a file.
	\param buffer [in] byte buffer
	\param filePath [in] path of the file to write.
	\return win32 error code like ERROR_ACCESS_DENIED,...
	*/
	uint32_t YTATextRW_WriteBytes(PCYTAByteBuffer buffer, const wchar_t* filePath);

	/*!
	\brief read wchar_t text from a file. buffer->buffer is resize/allocated automatically.
	\param buffer [out] wide char buffer
	\param filePath [in] path of the file to read
	\return win32 error code like ERROR_FILE_NOT_FOUND
	*/
	uint32_t YTATextRW_ReadWText(PYTAWTextBuffer buffer, const wchar_t* filePath);


	/*!
	\brief write wchar_t text to a file.
	\param buffer [in] wide char buffer
	\param filePath [in] path of the file to write.
	\return win32 error code like ERROR_ACCESS_DENIED,...
	*/
	uint32_t YTATextRW_WriteWText(PCYTAWTextBuffer buffer, const wchar_t* filePath);
#if defined(__cplusplus)
}
#endif
