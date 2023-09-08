#include "Logger.h"

#include <stdio.h>

using namespace Mazeline;

Logger::Logger(const char* logFilePath) {
	m_logFile = new File(logFilePath);
	if (!FileExists(logFilePath)) {
		m_logFile->Open(FileOpenMode::Write);
		m_logFile->Close();
	}
}

void Logger::AttachToFile() {
	if (!m_logFile->Exists())
		return;
	m_logFile->Open((FileOpenMode)(FileOpenMode::Append | FileOpenMode::Update));
}

void Logger::DetachFromFile() {
	m_logFile->Flush();
	m_logFile->Close();
}

void Logger::Log(LogLevel level, char* message, ...) {
	va_list args;
	va_start(args, message);
	vprintf(message, args);
	va_end(args);
}

void Logger::Log(LogLevel level, char* message, va_list& args) {
	if (m_logFile->IsOpen())
		vfprintf(m_logFile->GetStream(), message, args);
#ifndef NDEBUG
	vprintf(message, args);
#endif
}

Logger::~Logger() {
	delete m_logFile;
}