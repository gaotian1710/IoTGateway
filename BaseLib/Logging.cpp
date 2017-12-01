#include "stdafx.h"
#include "BaseDefs.h"
#include "Buffer.h"
#define _Logging_c
#include "Logging.h"

// global variables
YTAWTextBuffer _Logging_FilePath = { 0 };
YTAWTextBuffer _Logging_Buffer = { 0 };
CRITICAL_SECTION _Logging_Cs = { 0 };
Logging_Level _Logging_Level = Logging_LevelDebug;

PYTAWTextBuffer Logging_GetBuffer()
{
	return &_Logging_Buffer;
}
void Logging_Begin(LPCWSTR filePath, Logging_Level level)
{
	YTAWTextBuffer_Init(&_Logging_FilePath, MAX_PATH);
	YTABaseDefs_SetLocation(NULL, 0);
	wcscpy_s(_Logging_FilePath.buffer, _Logging_FilePath.ccLength, filePath);
	YTAWTextBuffer_Init(&_Logging_Buffer, 2048);
	InitializeCriticalSection(&_Logging_Cs);
	EnterCriticalSection(&_Logging_Cs);
	FILE* pFile;
	int ierr = _wfopen_s(&pFile, _Logging_FilePath.buffer, L"w, ccs=UTF-16LE");
	if (ierr) {
		MessageBox(NULL, L"could not open log file", L"Fatal error", MB_OK | MB_ICONERROR);
		exit(-1);
	}
	fwprintf(pFile, L"%wc", 0xfeff);
	fclose(pFile);
	_Logging_Level = level;
	LeaveCriticalSection(&_Logging_Cs);
}


void Logging_End()
{
	YTAWTextBuffer_Destroy(&_Logging_FilePath);
	YTAWTextBuffer_Destroy(&_Logging_Buffer);
	DeleteCriticalSection(&_Logging_Cs);
}

void _Logging_Write()
{
	FILE* pFile;
	int ierr = _wfopen_s(&pFile, _Logging_FilePath.buffer, 
		L"a, ccs=UTF-16LE");
	fwprintf(pFile, L"%ws\n", _Logging_Buffer.buffer);
	fclose(pFile);
	_Logging_Buffer.ccFilled = 0;
	LeaveCriticalSection(&_Logging_Cs);
}

static SYSTEMTIME _Logging_Time;
static const LPCWSTR _Logging_LabelErrorLevels[] = {
	L"DEB", L"INF", L"ERR"
};

void _Logging_WriteTime(Logging_Level level)
{
	if (level < _Logging_Level) return;
	EnterCriticalSection(&_Logging_Cs);
	SYSTEMTIME *st = &_Logging_Time;
	GetLocalTime(st);
	size_t availableLength = _Logging_Buffer.ccLength - _Logging_Buffer.ccFilled;
	_Logging_Buffer.ccFilled += _snwprintf_s(
		_Logging_Buffer.buffer + _Logging_Buffer.ccFilled, availableLength, availableLength - 1,
		L"%04d-%02d-%02d %02d:%02d:%02d.%03d,%ws,",
		st->wYear, st->wMonth, st->wDay, st->wHour, st->wMinute, st->wSecond, st->wMilliseconds,
		_Logging_LabelErrorLevels[level]
	);
}

void _Logging_WriteTimeAndLocation(Logging_Level level)
{
	if (level < _Logging_Level) return;
	EnterCriticalSection(&_Logging_Cs);
	SYSTEMTIME *st = &_Logging_Time;
	GetLocalTime(st);
	size_t availableLength = _Logging_Buffer.ccLength - _Logging_Buffer.ccFilled;
	_Logging_Buffer.ccFilled += _snwprintf_s(
		_Logging_Buffer.buffer + _Logging_Buffer.ccFilled, availableLength, availableLength - 1,
		L"%04d-%02d-%02d %02d:%02d:%02d.%03d,%ws,%hs,%d,",
		st->wYear, st->wMonth, st->wDay, st->wHour, st->wMinute, st->wSecond, st->wMilliseconds,
		_Logging_LabelErrorLevels[level], YTABaseDefs_GetFile(),YTABaseDefs_GetLine()
	);
}
