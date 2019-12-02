#pragma once

#include "stdafx.h"

// Represents the log levels
typedef enum class Log_Levels
{
	Log_Level_Info = 0,
	Log_Level_Warning = 1,
	Log_Level_Error = 2,
	Log_Level_Debug1 = 3,
	Log_Level_Debug2 = 4
}LogLevel;


// Represents the log type
typedef enum class Log_Types
{
	No_Log = 1,
	Console_Log = 2,
	File_Log = 3
}LogType;


/*
	Class used to store informations about the T.U. and allocator (a logger). 
	The logger stores information based on the level of the log:
	For Log_Level_Info stores information about: the size of the memory pool, start address, numar of allocation/deallocations
	For Log_Level_warning stores informations about: deallocating invalid addresses, memory leaks
	For Log_Level_Error stores informations about: cases when the program fails due to bad_alloc
	For Log_Level_Debug1 stores informations about: memory available, memory needed to allocate, 
												   memory available after allocation
	For Log_Level_Debug2 stored informations about: adds the list of memory available to the Debug1

	At the end of the program all the informations will be written into a file LogFile.log
*/
class Logger
{
public:
	Logger();

	void updateLog(const std::string& message, LogLevel LogLevel);

	void increaseAllocations();
	void increaseDeallocations();

	std::string tupletsAdressAndSize(const std::list<PoolElement>& mAvailable, const LogLevel& LogLevel);

	~Logger();
private:
	static std::string getCurrentTime();
	void writingToFile();
	void writingToConsole();

public:
	int totalMemoryAvailable;

private:
	std::ofstream m_loggerFile;
	std::vector<bool> m_logLevels;
	LogType m_logType;

	std::vector<std::string> outputMessages;

	int numberAllocations;
	int numberDeallocations;
};