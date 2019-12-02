#include "stdafx.h"
#pragma warning (disable : 4996)


/*
	Constructor used to open the file log, and initializate the data members
*/
Logger::Logger() : numberAllocations(0), numberDeallocations(0), totalMemoryAvailable(0)
{
	m_loggerFile.open("LogFile.log", std::ofstream::out);

	m_logLevels.resize(5);
	m_logLevels[(int)Log_Levels::Log_Level_Info] = true;
	m_logLevels[(int)Log_Levels::Log_Level_Warning] = true;
	m_logLevels[(int)Log_Levels::Log_Level_Error] = true;
    m_logLevels[(int)Log_Levels::Log_Level_Debug1] = true;
	//m_logLevels[(int)Log_Levels::Log_Level_Debug2] = true;

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

	outputMessages.resize(5);
}


/*
	Method used to update the log depending on the log level
*/
void Logger::updateLog(const std::string& message, LogLevel LogLevel)
{
	if ((m_logType == LogType::No_Log) || (!m_logLevels[(int)LogLevel]))
	{
		return;
	}
	if (LogLevel == LogLevel::Log_Level_Debug2)
	{
		LogLevel = LogLevel::Log_Level_Debug1;
	}

	outputMessages[(int)LogLevel] += Logger::getCurrentTime();
	outputMessages[(int)LogLevel] += "\t";

	switch (LogLevel)
	{
		
	case LogLevel::Log_Level_Debug1:
	case LogLevel::Log_Level_Debug2:
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
	Method used to increase the number of allocations that occur during the application
*/
void Logger::increaseAllocations()
{
	numberAllocations++;
}


/*
	Method used to increase the number of deallocations that occur during the application
*/
void Logger::increaseDeallocations()
{
	numberDeallocations++;
}


/*
	Method returns a string that is composed of tuplets (address, size) that represent all the 
	address and the size of them that are available to be allocated
	Method used for Debugging
*/
std::string Logger::tupletsAdressAndSize(const std::list<PoolElement>& mAvailable, const LogLevel& LogLevel)
{
	if (!m_logLevels[(int)LogLevel])
	{
		return "";
	}

	std::string memoryAndSize;

	for (auto it = mAvailable.begin(); it != mAvailable.end(); it++)
	{
		std::stringstream ss;
		ss << "(" << static_cast<void*>(it->address) << "," << it->size << ") \t";
		memoryAndSize += ss.str();
	}

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
	for (int currLevel = 0; currLevel < 5; currLevel++)
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
	for (int currLevel = 0; currLevel < 5; currLevel++)
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