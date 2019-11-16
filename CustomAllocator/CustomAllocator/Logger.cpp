#include "stdafx.h"
#pragma warning (disable : 4996)


Logger::Logger()
{
	m_loggerFile.open("LogFile.log", std::ofstream::out);
	m_logLevel = LogLevel::Log_Level_Info;
	m_logType = LogType::File_Log;

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
}


std::string Logger::getCurrentTime()
{
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);

	std::stringstream ss;
	ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
	return ss.str();
}


Logger::~Logger()
{
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
	m_loggerFile.close();
}