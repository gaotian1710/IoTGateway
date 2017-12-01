#include "stdafx.h"
#include "BaseDefs.h"
static const char* _file_;
static int _line_;
uint32_t _error_; // Win32 error code or COM HRESULT

void YTABaseDefs_SetLocation(const char* file, int line)
{
	_file_ = file;
	_line_ = line;
}

void YTABaseDefs_SetLocationAndClearError(const char * file, int line)
{
	YTABaseDefs_SetLocation(file, line);
	_error_ = ERROR_SUCCESS;
}

void YTABaseDefs_SetLocationWithLastError(const char * file, int line)
{
	YTABaseDefs_SetLocation(file, line);
	_error_ = GetLastError();
}


const char* YTABaseDefs_GetFile() { return _file_; }
uint32_t YTABaseDefs_ErrorCode()
{
	return _error_;
}
int YTABaseDefs_GetLine() { return _line_; }
