#pragma once

#include "stdafx.h"


typedef enum class Log_Levels
{
	Log_Level_Info = 0,
	Log_Level_Warning = 1,
	Log_Level_Error = 2,
}LogLevel;


typedef enum class Log_Types
{
	No_Log = 1,
	Console_Log = 2,
	File_Log = 3
}LogType;


/*
	This class is used to store informations about the T.U. and allocator. 
	The Logger stores information based on the level of the log:
		Log_Level_Info stores information about: the size of the memory pool, number of allocation and deallocations, memory available at a time, allocations size
		Log_Level_Warning stores informations about memory leaks
		Log_Level_Error stores informations about: cases when the program fails due to bad_alloc and when user is trying to deallocate an invalid addresses

	At the end of the program all the informations will be written into a file LogFile.log or written on screen
*/
class Logger
{
public:
	Logger(const size_t poolSize, const LogType logType);

	void updateWarningLog();
	void updateErrorLog(void *block, const size_t memoryToAllocate, const size_t biggestContMemory, const std::string& situation);

	void increaseAllocOrDealloc(const int size);

	~Logger();

private:
	void updateLog(const std::string& message, const LogLevel LogLevel);
	void writingToFile();
	void writingToConsole();
	static std::string getCurrentTime();

private:
	static int numberLogTypes;
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