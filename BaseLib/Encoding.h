#pragma once

/*!
\brief read a file as a binary stream.
\param filePath [in] file path
\param ppOut [out] buffer to put the data into
\return error code
*/
uint32_t YTAEncoding_ReadBytes(PCYTAWTextBuffer filePath, PYTAByteBuffer* ppOut);

/*!
\brief write binary byte array into a file.
\param filePath [in] file path
\param data [in] data to put into the file
\return error code
*/
uint32_t YTAEncoding_WriteBytes(PCYTAWTextBuffer filePath, PCYTAByteBuffer data);

/*!
\brief read wide character text file.
	if BOM does not exist or file length is odd, return ERROR_BAD_FORMAT.
\param filePath [in] file path
\param ppOut [out] buffer to put text into
\return error code
*/
uint32_t YTAEncoding_ReadWText(PCYTAWTextBuffer filePath, PYTAWTextBuffer* ppOut);

uint32_t YTAEncoding_WriteWText(PCYTAWTextBuffer filePath, PCYTAWTextBuffer data);

uint32_t YTAEncoding_W2OEM(PYTAByteBuffer* ppOut, PCYTAWTextBuffer pIn);

uint32_t YTAEncoding_W2U8(PYTAByteBuffer* ppOut, PCYTAWTextBuffer pIn);

uint32_t YTAEncoding_W2U8BOM(PYTAByteBuffer* ppOut, PCYTAWTextBuffer pIn);

uint32_t YTAEncoding_OEM2W(PYTAWTextBuffer* ppOut, PCYTAByteBuffer pIn);

uint32_t YTAEncoding_OEM2WBOM(PYTAWTextBuffer* ppOut, PCYTAByteBuffer pIn);

uint32_t YTAEncoding_U82W(PYTAWTextBuffer* ppOut, PCYTAByteBuffer pIn);

uint32_t YTAEncoding_U82WBOM(PYTAWTextBuffer* ppOut, PCYTAByteBuffer pIn);
