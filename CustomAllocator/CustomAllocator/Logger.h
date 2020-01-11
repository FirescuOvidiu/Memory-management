#pragma once

#include "stdafx.h"

// Represents the log levels
typedef enum class Log_Levels
{
	Log_Level_Info = 0,
	Log_Level_Warning = 1,
	Log_Level_Error = 2,
	Log_Level_Debug1 = 3,
	Log_Level_Debug2 = 4,
	Log_Level_Debug3 = 5
}LogLevel;


// Represents the log types
typedef enum class Log_Types
{
	No_Log = 1,
	Console_Log = 2,
	File_Log = 3
}LogType;


/*
	This class is used to store informations about the T.U. and allocator. 
	The Logger stores information based on the level of the log:
		For Log_Level_Info stores information about: the size of the memory pool, start address, numar of allocation/deallocations
		For Log_Level_warning stores informations about: deallocating invalid addresses, memory leaks
		For Log_Level_Error stores informations about: cases when the program fails due to bad_alloc
		For Log_Level_Debug1 stores informations about: memory available, memory needed to allocate, 
												   memory available after allocation
		For Log_Level_Debug2 stores informations stored in Log_Level_Debug1 and adds to them the list of memory available
		For Log_Level_Debug3 stores informations stored in Log_Level_Debug2 and adds to them the list of memory allocated

	At the end of the program all the informations will be written into a file LogFile.log
*/
class Logger
{
public:
	Logger();

	void initLogger(size_t poolSize, char* startAddresss);
	void updateWarningLog(size_t poolSize);
	void updateErrorLog(void *block, size_t memoryToAllocate, size_t biggestContMemory, const std::string& situation);
	void updateDebugLog(const std::string& message, const std::list<PoolElement>& mAvailable, const std::set<PoolElement>& mAllocated, bool end);

	void increaseAllocOrDealloc(const int size);

	~Logger();

private:
	void updateLog(const std::string& message, const LogLevel LogLevel);
	void writingToFile();
	void writingToConsole();
	std::string tupletsAdressAndSize(const std::list<PoolElement>& mAvailable, const std::set<PoolElement>& mAllocated, const LogLevel& LogLevel);
	static std::string getCurrentTime();

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