#include "DebugHelpers.h"
#include <chrono>
#include <stdio.h>
#include <rwgui.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <Windows.h>

void DebugHelpers::Logger::Log(char * LogText)
{
	PerformLog(ELogVerbosity::LV_Log, LogText);
}

void DebugHelpers::Logger::Warning(char * LogText)
{
	PerformLog(ELogVerbosity::LV_Warning,LogText);
}

void DebugHelpers::Logger::Error(char * LogText)
{
	PerformLog(ELogVerbosity::LV_Error, LogText);
}

void DebugHelpers::Logger::PerformLog(ELogVerbosity logVerbosity, char * LogText)
{
	char* logTypeText = "";
	switch (logVerbosity)
	{
	case ELogVerbosity::LV_Log:
		logTypeText = "Log";
		break;
	case ELogVerbosity::LV_Warning:
		logTypeText = "Warning";
		break;
	case ELogVerbosity::LV_Error:
		logTypeText = "Error";
	default:
		break;
	}


	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);

	std::stringstream ss;
	ss << std::put_time(std::localtime(&in_time_t), "%Y %B %e %H:%M:%S");
	const std::string tmp = ss.str();
	const char*	timeText = tmp.c_str();



	char msg[1024];
	ZeroMemory(msg, 1024);

	if (logVerbosity == ELogVerbosity::LV_Error)
	{
		sprintf(msg, "[%s] %s [%i] : %s\n ", timeText, logTypeText, GetLastError(), LogText);
	}
	else {
		sprintf(msg, "[%s] %s : %s\n", timeText, logTypeText, LogText);
	}
		
	char log_filepath[FILENAME_MAX];
	ZeroMemory(log_filepath, FILENAME_MAX);
	sprintf(log_filepath, "%s\\%s.log", GApplication->GetApplicationFolder(), GApplication->GetApplicationName());
	FILE* logFile = fopen(log_filepath, "a+");
	if (logFile)
	{
		fputs(msg, logFile);
		fseek(logFile, 0, SEEK_END);
		fflush(logFile);
	}
	fclose(logFile);

	OutputDebugString(msg);

	if (logVerbosity == ELogVerbosity::LV_Error)
	{
		DebugBreak();
		GApplication->Stop();
	};
}
