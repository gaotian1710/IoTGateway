#pragma once
class TestUtils
{
public:
	TestUtils();
	~TestUtils();

	static const int logBufferLength = 2048;
	static WCHAR logBuffer[logBufferLength];
	static PYTAWTextBuffer TestDataDir();
};

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define TestLog(...) { \
	swprintf_s(TestUtils::logBuffer, TestUtils::logBufferLength, __VA_ARGS__); \
	Logger::WriteMessage(TestUtils::logBuffer); }