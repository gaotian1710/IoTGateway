#pragma once
#pragma region debug-logger
// global variables

typedef enum {
	Logging_LevelDebug = 0,
	Logging_LevelInfo,
	Logging_LevelError
} Logging_Level;
#if defined(__cplusplus)
extern "C" {
#endif
	PYTAWTextBuffer Logging_GetBuffer();
	void Logging_Begin(LPCWSTR filePath, Logging_Level level);
	void Logging_End();

	void _Logging_Write();
	void _Logging_WriteTime(Logging_Level level);
	void _Logging_WriteTimeAndLocation(Logging_Level level);
#if defined(__cplusplus)
}
#endif

#define Logging_Debug(...) \
{ \
	_Logging_WriteTimeAndLocation(Logging_LevelDebug); \
	PYTAWTextBuffer buffer = Logging_GetBuffer(); \
	buffer->ccFilled += _snwprintf_s( \
		buffer->buffer + buffer->ccFilled, \
		buffer->ccLength - buffer->ccFilled, \
		buffer->ccLength - buffer->ccFilled - 1, __VA_ARGS__); \
	_Logging_Write(); \
}


#define Logging_Info(...) \
{ \
	_Logging_WriteTime(Logging_LevelInfo); \
	PYTAWTextBuffer buffer = Logging_GetBuffer(); \
	buffer->ccFilled += _snwprintf_s( \
		buffer->buffer + buffer->ccFilled, \
		buffer->ccLength - buffer->ccFilled, \
		buffer->ccLength - buffer->ccFilled - 1, __VA_ARGS__); \
	_Logging_Write(); \
}

#define Logging_Error(...) \
{ \
	_Logging_WriteTimeAndLocation(Logging_LevelError); \
	PYTAWTextBuffer buffer = Logging_GetBuffer(); \
	buffer->ccFilled += _snwprintf_s( \
		buffer->buffer + buffer->ccFilled, \
		buffer->ccLength - buffer->ccFilled, \
		buffer->ccLength - buffer->ccFilled - 1, __VA_ARGS__); \
	_Logging_Write(); \
}
#pragma endregion debug-logger

#pragma region realtime-trace-logger

#pragma endregion realtime-trace-logger
