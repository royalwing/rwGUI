#ifndef DEBUGHELPERS_H
#define DEBUGHELPERS_H

#include <rwapi.h>

namespace DebugHelpers
{
	class RWGUI_API Logger
	{
	private:
		Logger() {};
		~Logger() {};
	public:
		static Logger* Get() { static Logger instance; return &instance; };
		
		enum ELogVerbosity
		{
			LV_Log = 0,
			LV_Warning,
			LV_Error
		};

		void Log(char* LogText);
		void Warning(char* LogText);
		void Error(char* LogText);
		void PerformLog(ELogVerbosity logVerbosity, char* LogText);
	};
}

#define RW_LOG(x) DebugHelpers::Logger::Get()->Log(x)
#define RW_ERROR(x) DebugHelpers::Logger::Get()->Error(x)
#define RW_WARNING(x) DebugHelpers::Logger::Get()->Warning(x)


#endif // DEBUGHELPERS_H