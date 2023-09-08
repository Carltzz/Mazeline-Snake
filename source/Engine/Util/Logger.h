#ifndef M_LOGGER_H
#define M_LOGGER_H

#include "IO/File/File.h"

namespace Mazeline {
	
	enum LogLevel {
		Debug = 0,
		Info = 1,
		Warning = 2,
		Error = 3,
		Critical = 4
	};

	// A simple implementation of a logging system which writes to files
	class Logger {
	public:
		Logger(const char* logFilePath);
		~Logger();

		// Begins writing logs to the file
		void AttachToFile();

		// Stops writing logs to the file
		void DetachFromFile();

		// Logs a formatted message to the output stream
		void Log(LogLevel logLevel, char* message, ...);

		// Logs a formatted message to the output stream
		void Log(LogLevel logLevel, char* message, va_list& args);

	private:
		File* m_logFile;
	};
}
#endif