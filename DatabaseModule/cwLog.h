#pragma once
#include <string>
class cwLog
{
public:
	cwLog(void);
	~cwLog(void);

	enum LogLevel
	{
		L_TRACE,
		L_ERROR,
		L_FATAL
	};
	static LogLevel m_level;
	static void setLevel(LogLevel level);
	static void WriteLog(LogLevel level, std::string text);
};

