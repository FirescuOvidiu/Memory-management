#include "stdafx.h"


Logger::Logger()
{
	m_loggerFile.open("LogFile.log", std::ios::out | std::ios::app);
	m_logLevel = LogLevel::Log_Level_Info;
	m_logType = LogType::File_Log;
}


Logger::~Logger()
{
	m_loggerFile.close();
}