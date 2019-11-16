#pragma once

#include "stdafx.h"

typedef enum class Log_Levels
{
	Log_Level_Info = 0,
	Log_Level_Warning = 1,
	Log_Level_Debug = 2,
	Log_Level_Error = 3
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

	void updateLog(std::string message);
	void updateLogLevel(LogLevel newLogLevel);

	void updateLogStart(const std::list<PoolElement>& mAvailable);
	void updateLogEnd();

	~Logger();
private:
	std::string getCurrentTime();

public:
	int numberAllocations;
	int numberDeallocations;
private:
	std::ofstream m_loggerFile;
	LogLevel m_logLevel;
	LogType m_logType;

	std::vector<std::string> outputMessages;
};