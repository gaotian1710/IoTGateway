#include "stdafx.h"
#include "BaseDefs.h"
#include "Buffer.h"
#include "Encoding.h"

uint32_t YTAEncoding_ReadBytes(PCYTAWTextBuffer filePath, PYTAByteBuffer * ppOut)
{
	uint32_t err = ERROR_SUCCESS;
	HANDLE hFile = NULL;
	YTAByteBuffer_Delete(ppOut);
	do {
		if (!PathFileExists(filePath->buffer))
		{
			err = ERROR_FILE_NOT_FOUND;
			break;
		}
		hFile = CreateFile(filePath->buffer, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			err = GetLastError();
			break;
		}
		DWORD fileSize = GetFileSize(hFile, NULL);
		*ppOut = YTAByteBuffer_New((uint32_t)(fileSize + 1));
		PYTAByteBuffer pOut = *ppOut;
		do {
			DWORD readLength = 0;
			BOOL b = ReadFile(hFile, pOut->buffer + pOut->bcFilled, fileSize, &readLength, NULL);
			if (!b)
			{
				err = GetLastError();
				if (err != ERROR_IO_PENDING)
				{
					break;
				}
				else
				{
					err = ERROR_SUCCESS;
				}
			}
			fileSize -= readLength;
			pOut->bcFilled += readLength;
		} while (fileSize);
	} while (0);

	if (hFile != NULL && hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
	if (err)
	{
		if (*ppOut)
		{
			YTAByteBuffer_Delete(ppOut);
		}
	}
	return err;
}

uint32_t YTAEncoding_WriteBytes(PCYTAWTextBuffer filePath, PCYTAByteBuffer data)
{
	uint32_t err = ERROR_SUCCESS;
	HANDLE hFile = NULL;
	do {
		hFile = CreateFile(filePath->buffer, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			err = GetLastError();
			break;
		}

		DWORD remaining = (DWORD)(data->bcFilled);
		const uint8_t* pData = data->buffer;
		do {
			DWORD writtenLength = 0;
			BOOL b = WriteFile(hFile, pData, remaining, &writtenLength, NULL);
			if (!b)
			{
				err = GetLastError();
				if (err != ERROR_IO_PENDING)
				{
					break;
				}
				else
				{
					err = ERROR_SUCCESS;
				}
			}
			remaining -= writtenLength;
			pData += writtenLength;
		} while (remaining);
	} while (0);
	if (hFile != NULL && hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
	}
	return err;
}

uint32_t YTAEncoding_ReadWText(PCYTAWTextBuffer filePath, PYTAWTextBuffer * ppOut)
{
	uint32_t err = ERROR_SUCCESS;
	HANDLE hFile = NULL;
	YTAWTextBuffer_Delete(ppOut);
	do {
		if (!PathFileExists(filePath->buffer))
		{
			err = ERROR_FILE_NOT_FOUND;
			break;
		}
		hFile = CreateFile(filePath->buffer, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			err = GetLastError();
			break;
		}
		DWORD fileSize = GetFileSize(hFile, NULL);
		if (fileSize & 1)
		{ // file size is odd. impossible!
			err = ERROR_BAD_FORMAT; break;
		}
		
		*ppOut = YTAWTextBuffer_New((size_t)fileSize/sizeof(wchar_t));
		PYTAWTextBuffer pOut = *ppOut;
		do {
			DWORD readLength = 0;
			BOOL b = ReadFile(hFile, (void*)(pOut->buffer + pOut->ccFilled), fileSize, &readLength, NULL);
			if (!b)
			{
				err = GetLastError();
				if (err != ERROR_IO_PENDING)
				{
					break;
				}
				else
				{
					err = ERROR_SUCCESS;
				}
			}
			fileSize -= readLength;
			pOut->ccFilled += readLength / sizeof(wchar_t);
		} while (fileSize);
	} while (0);
	if (hFile != NULL && hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
	}
	if (err)
	{
		YTAWTextBuffer_Delete(ppOut);
	}
	return err;
}
