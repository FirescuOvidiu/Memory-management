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
		For Log_Level_Info stores information about: the size of the memory pool, numar of allocation and deallocations,
		memory available at a time, allocations size
		For Log_Level_warning stores informations about memory leaks
		For Log_Level_Error stores informations about: cases when the program fails due to bad_alloc and 
		cases when the program fails due to user trying to deallocate an invalid addresses

	At the end of the program all the informations will be written into a file LogFile.log or written on screen
*/
class Logger
{
public:
	Logger() : numberAllocations(0), numberDeallocations(0), totalMemory(0), totalMemoryAvailable(0), logType(LogType::File_Log) {}

	void initLogger(size_t poolSize);
	void updateWarningLog();
	void updateErrorLog(void *block, size_t memoryToAllocate, size_t biggestContMemory, const std::string& situation);

	void increaseAllocOrDealloc(const int size);

	~Logger();

private:
	void updateLog(const std::string& message, const LogLevel LogLevel);
	void writingToFile();
	void writingToConsole();
	static std::string getCurrentTime();

private:
	std::ofstream allocationsSizeFile;
	std::ofstream loggerFile;
	std::vector<bool> logLevels;
	LogType logType;

	std::vector<std::string> outputMessages;

	int numberAllocations;
	int numberDeallocations;
	int totalMemoryAvailable;
	int totalMemory;
};