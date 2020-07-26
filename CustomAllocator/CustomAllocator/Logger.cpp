#include "stdafx.h"
#pragma warning (disable : 4996)


int Logger::numberLogTypes = 3;


Logger::Logger(const size_t poolSize, const LogType logType) : numberAllocations(0), numberDeallocations(0), totalMemory((int)poolSize), totalMemoryAvailable((int)poolSize), logType(logType)
{
	if (logType == LogType::No_Log)
	{
		return;
	}

	loggerFile.open("LogFile.log", std::ofstream::out);
	allocationsSizeFile.open("sizeAllocations.txt", std::ofstream::out);

	logLevels.resize(numberLogTypes, true);
	outputMessages.resize(numberLogTypes);

	outputMessages[(int)LogLevel::Log_Level_Info] += "\n\n";
	updateLog("<----------------------- START OF APPLICATION ----------------------->\n", LogLevel::Log_Level_Info);
	updateLog("Size of the memory pool: " + std::to_string(poolSize) + " bytes.", LogLevel::Log_Level_Info);
}


/*
	Method used to update the log depending on the log level
*/
void Logger::updateLog(const std::string& message, const LogLevel LogLevel)
{
	if ((logType == LogType::No_Log) || (message == ""))
	{
		return;
	}

	outputMessages[(int)LogLevel] += Logger::getCurrentTime();
	outputMessages[(int)LogLevel] += "\t";

	switch (LogLevel)
	{
	case LogLevel::Log_Level_Info:
		outputMessages[(int)LogLevel::Log_Level_Info] += "[INFO]";
		break;

	case LogLevel::Log_Level_Warning:
		outputMessages[(int)LogLevel::Log_Level_Warning] += "[WARNINIG]";
		break;

	case LogLevel::Log_Level_Error:
		outputMessages[(int)LogLevel::Log_Level_Error] += "[ERROR]";
		break;

	default:
		break;
	}

	outputMessages[(int)LogLevel] += "\t";
	outputMessages[(int)LogLevel] += message;
	outputMessages[(int)LogLevel] += "\n";
}


/*
	Method used to update the log for the following level: Log_level_warning
*/
void Logger::updateWarningLog()
{
	if (logType == LogType::No_Log)
	{
		return;
	}

	updateLog("The application has memory leaks !!", LogLevel::Log_Level_Warning);
	updateLog("The size of the memory allocated that wasn't deallocated: " + std::to_string(totalMemory - totalMemoryAvailable) + " bytes.", LogLevel::Log_Level_Warning);
}


/*
	Method used to update the log for the following level: Log_Level_Error
*/
void Logger::updateErrorLog(void* block, const size_t memoryToAllocate, const size_t biggestContMemory, const std::string& situation)
{
	if (logType == LogType::No_Log)
	{
		return;
	}

	if (situation == "Bad alloc")
	{
		updateLog("Bad alloc because the biggest continuous memory is smaller than the memory request.", LogLevel::Log_Level_Error);
		updateLog("Memory available: " + std::to_string(totalMemoryAvailable), LogLevel::Log_Level_Error);
		updateLog("Memory needed: " + std::to_string(memoryToAllocate), LogLevel::Log_Level_Error);
		updateLog("Biggest continuous memory: " + std::to_string(biggestContMemory), LogLevel::Log_Level_Error);
	}

	if (situation == "Invalid Address")
	{
		std::stringstream ss;
		ss << block;

		updateLog("The application tries to deallocate an address that's not allocated. Adress: " + ss.str() + "\n", LogLevel::Log_Level_Error);
	}
}


/*
	Method used to increase the number of allocations or deallocations that occur during the application
	and to update the total memory available
*/
void Logger::increaseAllocOrDealloc(const int size)
{
	if (logType == LogType::No_Log)
	{
		return;
	}

	if (size < 0)
	{
		numberAllocations++;
		allocationsSizeFile << -size << "\n";
	}
	else
	{
		numberDeallocations++;
	}
	totalMemoryAvailable += size;
}


/*
	Returns the current time based on the format YYYY-MM-DD HH:MM:SS
*/
std::string Logger::getCurrentTime()
{
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);

	std::stringstream ss_currTime;
	ss_currTime << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
	return ss_currTime.str();
}


void Logger::writingToFile()
{
	for (const auto& message : outputMessages)
	{
		if (message != "")
		{
			loggerFile << message << "\n\n\n";
		}
	}
}


void Logger::writingToConsole()
{
	for (const auto& message : outputMessages)
	{
		if (message != "")
		{
			std::cout << message << "\n\n\n";
		}
	}
}


Logger::~Logger()
{
	if (logType == LogType::No_Log)
	{
		return;
	}

	Logger::updateLog("Number of allocations during application: " + std::to_string(numberAllocations), LogLevel::Log_Level_Info);
	Logger::updateLog("Number of deallocations during application: " + std::to_string(numberDeallocations) + "\n", LogLevel::Log_Level_Info);
	Logger::updateLog("<----------------------- END OF APPLICATION ----------------------->", LogLevel::Log_Level_Info);

	switch (logType)
	{
	case LogType::File_Log:
		writingToFile();
		break;

	case LogType::Console_Log:
		writingToConsole();
		break;

	default:
		break;
	}


	if (loggerFile.is_open())
	{
		loggerFile.close();
	}
	if (allocationsSizeFile.is_open())
	{
		allocationsSizeFile.close();
	}
}