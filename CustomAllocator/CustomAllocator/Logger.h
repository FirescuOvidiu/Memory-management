#pragma once

#include "stdafx.h"

// Represents the log levels
typedef enum class Log_Levels
{
	Log_Level_Info = 0,
	Log_Level_Warning = 1,
	Log_Level_Error = 2,
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

	At the end of the program all the informations will be written into a file LogFile.log
*/
class Logger
{
public:
	Logger();

	void initLogger(size_t poolSize, char* startAddresss);
	void updateWarningLog(size_t poolSize);
	void updateErrorLog(void *block, size_t memoryToAllocate, size_t biggestContMemory, const std::string& situation);

	void increaseAllocOrDealloc(const int size);

	~Logger();

private:
	void updateLog(const std::string& message, const LogLevel LogLevel);
	void writingToFile();
	void writingToConsole();
	static std::string getCurrentTime();

private:
	std::ofstream m_loggerFile;
	std::vector<bool> m_logLevels;
	LogType m_logType;

	std::vector<std::string> outputMessages;
	std::vector<int> countBlocksAllocated;

	int numberAllocations;
	int numberDeallocations;
	int totalMemoryAvailable;
	int totalMemory;
};