#include "stdafx.h"
#include "BaseDefs.h"
#include "Buffer.h"
#include "TextRW.h"

uint32_t YTATextRW_ReadBytes(PYTAByteBuffer buffer, const wchar_t * filePath)
{
	uint32_t err = ERROR_SUCCESS;
	HANDLE hFile = NULL;
	do {
		CREATEFILE2_EXTENDED_PARAMETERS cf2ExParams = {
			sizeof(CREATEFILE2_EXTENDED_PARAMETERS),
			FILE_ATTRIBUTE_NORMAL,
			0, // dwFileFlags
			SECURITY_IMPERSONATION,
			NULL, // lpSecurityAttributes
			NULL, // hTemplateFile
		};
		
		hFile = CreateFile2(filePath, GENERIC_READ, 0, OPEN_EXISTING, &cf2ExParams);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			err = GetLastError(); break;
		}
		LARGE_INTEGER llFileSize = { 0 };
		BOOL b = GetFileSizeEx(hFile, &llFileSize);
		if (!b)
		{
			err = GetLastError(); break;
		}
		if (llFileSize.LowPart > (DWORD)(buffer->bcLength))
		{ // Resize buffer
			YTAByteBuffer_Destroy(buffer);
			if (ERROR_SUCCESS != (err = YTAByteBuffer_Init(buffer, llFileSize.LowPart)))
			{
				break;
			}
		}
		else
		{
			buffer->bcFilled = 0; // clear buffer
		}

		DWORD remainingBytes = llFileSize.LowPart;
		do {
			DWORD readBytes = 0;
			b = ReadFile(hFile, buffer->buffer + buffer->bcFilled, remainingBytes, &readBytes, NULL);
			if (!b)
			{
				if (ERROR_IO_PENDING != (err = GetLastError()))
				{
					break;
				}
				else
				{
					err = ERROR_SUCCESS;
				}
			}
			remainingBytes -= readBytes;
			buffer->bcFilled += readBytes;
		} while (remainingBytes);
	} while (0);
	if (hFile != NULL && hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
	}
	return err;
}

uint32_t YTATextRW_WriteBytes(PCYTAByteBuffer buffer, const wchar_t * filePath)
{
	uint32_t err = ERROR_SUCCESS;
	HANDLE hFile;
	do {
		CREATEFILE2_EXTENDED_PARAMETERS cf2ExParams = {
			sizeof(CREATEFILE2_EXTENDED_PARAMETERS),
			FILE_ATTRIBUTE_NORMAL,
			0, // dwFileFlags
			SECURITY_IMPERSONATION,
			NULL, // lpSecurityAttributes
			NULL, // hTemplateFile
		};

		hFile = CreateFile2(filePath, GENERIC_WRITE, 0, CREATE_ALWAYS, &cf2ExParams);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			err = GetLastError(); break;
		}

		DWORD remainingBytes = buffer->bcFilled;
		uint8_t* ptrToWrite = buffer->buffer;
		do {
			DWORD writtenBytes = 0;
			if (!WriteFile(hFile, ptrToWrite, remainingBytes, &writtenBytes, NULL))
			{
				if (ERROR_IO_PENDING != (err = GetLastError()))
				{
					break;
				}
				else
				{
					err = ERROR_SUCCESS;
				}
			}
			ptrToWrite += writtenBytes;
			remainingBytes -= writtenBytes;
		} while (remainingBytes);
	} while (0);
	if (hFile != NULL && hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
	}
	return err;
}

uint32_t YTATextRW_ReadWText(PYTAWTextBuffer buffer, const wchar_t * filePath)
{
	uint32_t err = ERROR_SUCCESS;
	YTAByteBuffer byteBuffer = { 0 };
	do {
		if (ERROR_SUCCESS != (err = YTATextRW_ReadBytes(&byteBuffer, filePath)))
		{
			break;
		}
		uint32_t estimateCharCount = (byteBuffer.bcFilled - 2)/sizeof(WCHAR);
		wchar_t bom = 0xfeff;
		if (bom != *(wchar_t*)(byteBuffer.buffer)) { // valid BOM not found
			err = ERROR_BAD_FORMAT;
			break;
		}
		if ((buffer->ccLength) < (estimateCharCount + 1))
		{
			if (ERROR_SUCCESS != (err = YTAWTextBuffer_Init(buffer, estimateCharCount + 1)))
			{
				break;
			}
		}
		const wchar_t* src = (wchar_t*)(byteBuffer.buffer) + 1;
		wcsncpy_s(buffer->buffer, buffer->ccLength, src, estimateCharCount);
		buffer->ccFilled = estimateCharCount;
	} while (0);
	YTAByteBuffer_Destroy(&byteBuffer);
	return err;
}

uint32_t YTATextRW_WriteWText(PCYTAWTextBuffer buffer, const wchar_t * filePath)
{
	uint32_t err = ERROR_SUCCESS;
	YTAByteBuffer byteBuffer = { 0 };
	do {
		uint32_t byteBufferSize = (buffer->ccFilled + 1) * sizeof(wchar_t);
		YTAByteBuffer_Init(&byteBuffer, byteBufferSize);
		wchar_t bom = 0xfeff;
		if (buffer->buffer[0] == bom)
		{
			size_t copyLength = buffer->ccFilled * sizeof(wchar_t);
			memcpy_s((void*)byteBuffer.buffer, (size_t)byteBuffer.bcLength, (const void*)(buffer->buffer), copyLength);
			byteBuffer.bcFilled = (uint32_t)copyLength;
		}
		else
		{
			*(wchar_t*)(byteBuffer.buffer) = bom;
			byteBuffer.bcFilled = sizeof(wchar_t);
			size_t copyLength = sizeof(wchar_t) * (buffer->ccFilled);
			memcpy_s((void*)(byteBuffer.buffer + byteBuffer.bcFilled), byteBuffer.bcLength - byteBuffer.bcFilled,
				(const void*)(buffer->buffer), copyLength);
			byteBuffer.bcFilled += (uint32_t)copyLength;
		}
		err = YTATextRW_WriteBytes(&byteBuffer, filePath);
	} while (0);
	YTAByteBuffer_Destroy(&byteBuffer);
	return err;
}
