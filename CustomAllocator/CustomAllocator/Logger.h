#pragma once

#include "stdafx.h"

typedef enum class Log_Levels
{
	Disable_Log = 1,
	Log_Level_Info = 2,
	Log_Level_Warning = 3,
	Log_Level_Error = 4,
	Log_Level_Debug = 5
}LogLevel;


typedef enum class Log_Types
{
	No_Log = 1,
	Console_Log = 2,
	File_Log = 3
}LogType;


class Logger
{
public:
	Logger();
	~Logger();
private:
	std::string getCurrentTime();
private:
	std::ofstream m_loggerFile;
	LogLevel m_logLevel;
	LogType m_logType;
};