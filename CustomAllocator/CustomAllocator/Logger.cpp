#include "stdafx.h"
#pragma warning (disable : 4996)


/*
	Constructor used to open the file log and initialize data members
	When setting Log_Level_Debug to a higher Log_Level_Debug make all the previous debug levels true
*/
Logger::Logger() : numberAllocations(0), numberDeallocations(0), totalMemoryAvailable(0)
{
	m_loggerFile.open("LogFile.log", std::ofstream::out);

	m_logLevels.resize(6);
	m_logLevels[(int)Log_Levels::Log_Level_Info] = true;
	m_logLevels[(int)Log_Levels::Log_Level_Warning] = true;
	m_logLevels[(int)Log_Levels::Log_Level_Error] = true;
    m_logLevels[(int)Log_Levels::Log_Level_Debug1] = true;
	m_logLevels[(int)Log_Levels::Log_Level_Debug2] = true;
	m_logLevels[(int)Log_Levels::Log_Level_Debug3] = true;

	m_logType = LogType::File_Log;

	if (m_logType == LogType::No_Log)
	{
		return;
	}

	switch (m_logType)
	{
	case LogType::File_Log:
		m_loggerFile << Logger::getCurrentTime() << "\t" << "[INFO]" << "\t" << "<----------------------- START OF APPLICATION ----------------------->" << "\n\n\n\n\n";
		break;
	case LogType::Console_Log:
		std::cout << Logger::getCurrentTime() << "\t" << "[INFO]" << "\t" << "<----------------------- START OF APPLICATION ----------------------->" << "\n\n\n\n\n";
		break;
	default:
		break;
	}

	outputMessages.resize(4);
}


/*
	Method used to update the log depending on the log level
*/
void Logger::updateLog(const std::string& message, const LogLevel LogLevel)
{
	if ((m_logType == LogType::No_Log) || (!m_logLevels[(int)LogLevel]) || (message == ""))
	{
		return;
	}

	outputMessages[(int)LogLevel] += Logger::getCurrentTime();
	outputMessages[(int)LogLevel] += "\t";

	switch (LogLevel)
	{
		
	case LogLevel::Log_Level_Debug1:
		outputMessages[(int)LogLevel::Log_Level_Debug1] += "[DEBUG]";
		break;

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
	Method used to update the log for the following levels: Log_Level_Debug1, Log_Level_Debug2, Log_Level_Debug3
*/
void Logger::updateDebugLog(const std::string& message, const std::list<PoolElement>& mAvailable, const std::set<PoolElement>& mAllocated, bool end)
{

	updateLog(message, LogLevel::Log_Level_Debug1);
	updateLog(tupletsAdressAndSize(mAvailable, mAllocated, LogLevel::Log_Level_Debug2), LogLevel::Log_Level_Debug1);
	updateLog(tupletsAdressAndSize(mAvailable, mAllocated, LogLevel::Log_Level_Debug3), LogLevel::Log_Level_Debug1);

	if (end)
	{
		outputMessages[(int)LogLevel::Log_Level_Debug1] += "\n";
	}
}


/*
	Method used to update the log for the following level: Log_Level_Info
*/
void Logger::updateInfoLog(size_t poolSize, char* startAddress)
{

	updateLog("Size of the memory pool: " + std::to_string(poolSize) + " bytes.", LogLevel::Log_Level_Info);
	totalMemoryAvailable = (int)poolSize;

	std::stringstream ss;
	ss << static_cast<void*>(startAddress);
	updateLog("Start address: " + ss.str(), LogLevel::Log_Level_Info);
}


/*
	Method used to update the log for the following level: Log_Level_Error
*/
void Logger::updateErrorLog(size_t memoryToAllocate, size_t biggestContMemory)
{
	updateLog("Bad alloc because the biggest continuous memory is smaller than the memory request.", LogLevel::Log_Level_Error);
	updateLog("Memory available: " + std::to_string(totalMemoryAvailable), LogLevel::Log_Level_Error);
	updateLog("Memory needed: " + std::to_string(memoryToAllocate), LogLevel::Log_Level_Error);
	updateLog("Biggest continuous memory: " + std::to_string(biggestContMemory), LogLevel::Log_Level_Error);
}


/*
	Method used to increase the number of allocations that occur during the application
	and to update the total memory available
*/
void Logger::increaseAllocations(int size)
{
	numberAllocations++;
	totalMemoryAvailable -= size;
}


/*
	Method used to increase the number of deallocations that occur during the application
	and to update the total memory available
*/
void Logger::increaseDeallocations(int size)
{
	numberDeallocations++;
	totalMemoryAvailable += size;
}


/*
	Method returns a string that is composed of tuplets (address, size) that represent 
	- all the address and the size which are available to be allocated
	- or all the adress and the size which are allocated depending on the LogLevel
	Method used for Debugging
*/
std::string Logger::tupletsAdressAndSize(const std::list<PoolElement>& mAvailable, const std::set<PoolElement>& mAllocated, const LogLevel& LogLevel)
{
	if (!m_logLevels[(int)LogLevel])
	{
		return "";
	}

	std::stringstream ss_currTuplet;
	std::string memoryAndSize;

	if (LogLevel == LogLevel::Log_Level_Debug2)
	{
		memoryAndSize += "Memory Available: ";
		for (auto it = mAvailable.begin(); it != mAvailable.end(); it++)
		{
			ss_currTuplet << "(" << static_cast<void*>(it->address) << "," << it->size << ") \t";
		}
	}

	if (LogLevel == LogLevel::Log_Level_Debug3)
	{
		memoryAndSize += "Memory Allocated: ";
		for (auto it = mAllocated.begin(); it != mAllocated.end(); it++)
		{
			ss_currTuplet << "(" << static_cast<void*>(it->address) << "," << it->size << ") \t";
		}
	}

	memoryAndSize += ss_currTuplet.str();
	return memoryAndSize;
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


/*
	Function used to write the informations into the log file
*/
void Logger::writingToFile()
{
	for (int currLevel = 0; currLevel < outputMessages.size(); currLevel++)
	{
		if ((m_logLevels[currLevel]) && (outputMessages[currLevel] != ""))
		{
			m_loggerFile << outputMessages[currLevel] << "\n\n\n";
		}
	}

	m_loggerFile << Logger::getCurrentTime() << "\t" << "[INFO]" << "\t" << "<----------------------- END OF APPLICATION ----------------------->";
}


/*
	Function used to write the informations into the console
*/
void Logger::writingToConsole()
{
	for (int currLevel = 0; currLevel < outputMessages.size(); currLevel++)
	{
		if ((m_logLevels[currLevel]) && (outputMessages[currLevel] != ""))
		{
			std::cout << outputMessages[currLevel] << "\n\n\n";
		}
	}

	std::cout << Logger::getCurrentTime() << "\t" << "[INFO]" << "\t" << "<----------------------- END OF APPLICATION ----------------------->";
}


/*
	Destructor used to write the information gathered during the program into the log and closing the file log
*/
Logger::~Logger()
{
	if (m_logType != LogType::No_Log)
	{
		Logger::updateLog("Number of allocations during application: " + std::to_string(numberAllocations), LogLevel::Log_Level_Info);
		Logger::updateLog("Number of deallocations during application: " + std::to_string(numberDeallocations), LogLevel::Log_Level_Info);


		switch (m_logType)
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
	}
	m_loggerFile.close();
}