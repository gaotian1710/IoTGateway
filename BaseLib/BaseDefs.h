#pragma once
/*! \file BaseDefs.h
\brief very very common definitions
*/

/*!
\brief Generic callback method common to whole BaseLib
*/
typedef uint32_t (*YTACallback)(void* pvParam);
#if defined(__cplusplus)
#define __ARRAYSIZE__(a)	(sizeof(a)/sizeof(a[0]))
extern "C" {
#endif
	/*!
	\brief error handling function
	\param file [in] global variable _file is set to file
	\param line [in] global variable _line is set to line.
	*/
	void YTABaseDefs_SetLocation(const char* file, int line);
	void YTABaseDefs_SetLocationAndClearError(const char* file, int line);
	void YTABaseDefs_SetLocationWithLastError(const char* file, int line);
	const char* YTABaseDefs_GetFile();
	uint32_t YTABaseDefs_ErrorCode();
	int YTABaseDefs_GetLine();
#if defined(__cplusplus)
}
#endif
