#include "MazelineEngine.h"
#include "Input/InputManager.h"

using namespace Mazeline;

void MazelineFatalError(const char* msg) {
}

MazelineEngine::MazelineEngine() {
	m_hasInitd = false;
	m_initError = false;
	m_camera = 0;
}

MazelineEngine& MazelineEngine::GetInstance() {
	static MazelineEngine engine;
	return engine;
}

bool MazelineEngine::Initialise() {
	if (m_hasInitd || m_initError) return false;

	m_debugLogger = new Logger("debug.txt");
#ifndef NDEBUG
	m_debugLogger->AttachToFile();
#endif

	MDebugLog("-------------------------------");
	MDebugLog("Initialising Mazeline Engine...");

	m_initError |= !InputManager::GetInstance().Load();
	if (!m_initError) MDebugLog("Successfully loaded Input Manager");
	m_initError |= !DisplayManager::GetInstance().Load();
	if (!m_initError) MDebugLog("Successfully loaded Display Manager");
	m_hasInitd = !m_initError;

	if (m_hasInitd) MDebugLog("Successfully initialised Engine");
	return m_hasInitd;
}

bool MazelineEngine::Terminate() {
	DisplayManager::GetInstance().Unload();

#ifndef NDEBUG
	m_debugLogger->DetachFromFile();
#endif

	delete m_debugLogger;
	return true;
}

void MazelineEngine::DebugLog(char* msg, ...) {
	va_list args;
	va_start(args, msg);
	m_debugLogger->Log(LogLevel::Debug, msg, args);
	m_debugLogger->Log(LogLevel::Debug, "\n");
	va_end(args);
}

void MazelineEngine::SetCamera(SimpleCamera* camera) {
	if (m_camera)
		m_camera->Deactivate();
	m_camera = camera;
	m_camera->Activate();
}