#include "stdafx.h"
#pragma warning (disable : 4996)


Logger::Logger() : numberAllocations(0), numberDeallocations(0), totalMemoryAvailable(0)
{
	m_loggerFile.open("LogFile.log", std::ofstream::out);
	m_logLevel = LogLevel::Log_Level_Info;
	m_logType = LogType::File_Log;

	if (m_logType == LogType::No_Log)
	{
		return;
	}

	switch (m_logType)
	{
	case LogType::File_Log:
		m_loggerFile << Logger::getCurrentTime() << "\t" << "[INFO]" << "\t" << "<----------------------- START OF APPLICATION ----------------------->" << "\n\n";
		break;
	case LogType::Console_Log:
		std::cout << Logger::getCurrentTime() << "\t" << "[INFO]" << "\t" << "<----------------------- START OF APPLICATION ----------------------->" << "\n\n";
		break;
	default:
		break;
	}

	outputMessages.resize(4);
}


void Logger::updateLog(std::string message)
{
	if (m_logType == LogType::No_Log)
	{
		return;
	}

	outputMessages[(int)m_logLevel] += Logger::getCurrentTime();
	outputMessages[(int)m_logLevel] += "\t";
	switch (m_logLevel)
	{
	case LogLevel::Log_Level_Info:
		outputMessages[(int)LogLevel::Log_Level_Info] += "[INFO]";
		break;
	case LogLevel::Log_Level_Warning:
		outputMessages[(int)LogLevel::Log_Level_Warning] += "[WARNINIG]";
		break;
	case LogLevel::Log_Level_Debug:
		outputMessages[(int)LogLevel::Log_Level_Debug] += "[DEBUG]";
		break;
	case LogLevel::Log_Level_Error:
		outputMessages[(int)LogLevel::Log_Level_Error] += "[DEBUG]";
		break;
	default:
		break;
	}
	outputMessages[(int)m_logLevel] += "\t";
	outputMessages[(int)m_logLevel] += message;
	outputMessages[(int)m_logLevel] += "\n";
}


void Logger::updateLogLevel(LogLevel newLogLevel)
{
	this->m_logLevel = newLogLevel;
}


std::string Logger::tupletsAdressAndSize(const std::list<PoolElement>& mAvailable)
{
	std::string memoryAndSize;

	for (auto it = mAvailable.begin(); it != mAvailable.end(); it++)
	{
		std::stringstream ss;
		ss << "(" << static_cast<void*>(it->adress) << "," << it->size << ") \t";
		memoryAndSize += ss.str();
	}

	return memoryAndSize;
}


std::string Logger::getCurrentTime()
{
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);

	std::stringstream ss_currTime;
	ss_currTime << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
	return ss_currTime.str();
}


Logger::~Logger()
{
	if (m_logType != LogType::No_Log)
	{
		Logger::updateLogLevel(LogLevel::Log_Level_Info);
		Logger::updateLog("Number of allocations during application: " + std::to_string(numberAllocations));
		Logger::updateLog("Number of deallocations during application: " + std::to_string(numberDeallocations));

		for (std::vector<std::string>::const_iterator it = outputMessages.cbegin(); it != outputMessages.cend(); it++)
		{
			m_loggerFile << *it << "\n\n";
		}

		switch (m_logType)
		{
		case LogType::File_Log:
			m_loggerFile << "\n\n" << Logger::getCurrentTime() << "\t" << "[INFO]" << "\t" << "<----------------------- END OF APPLICATION ----------------------->";
			break;
		case LogType::Console_Log:
			std::cout << "\n\n" << Logger::getCurrentTime() << "\t" << "[INFO]" << "\t" << "<----------------------- END OF APPLICATION ----------------------->";
			break;
		default:
			break;
		}
	}
	m_loggerFile.close();
}