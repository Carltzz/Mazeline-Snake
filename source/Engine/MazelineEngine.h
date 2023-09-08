#ifndef M_MAZELINEENGINE_H
#define M_MAZELINEENGINE_H

#define alloc_char_wstr(dst, str) dst = new char[sizeof str] { str };

#include "DisplayManager.h"
#include "Util/Logger.h"
#include "Camera.h"

#define MDebugLog(message, ...) MazelineEngine::GetInstance().DebugLog(message, ##__VA_ARGS__);

namespace Mazeline {

	// Terminates the program with the given error
	void MazelineFatalError(const char* msg);

	// A singleton class consisting of all the engine sdk tools
	class MazelineEngine {
	public:
		// Returns the instance of the engine
		static MazelineEngine& GetInstance();

		// Initialises the engine if it hasn't done so yet
		bool Initialise();

		// Terminates the game engine (CALL AFTER ALL ENGINE SYSTEMS ARE NO LONGER IN USE)
		bool Terminate();

		void SetCamera(SimpleCamera* camera);

		void DebugLog(char* msg, ...);

		MazelineEngine(const MazelineEngine&) = delete;
		MazelineEngine& operator = (const MazelineEngine&) = delete;

		~MazelineEngine() { }
	private:
		MazelineEngine();

		bool m_hasInitd;
		bool m_initError;

		Logger* m_debugLogger;
		SimpleCamera* m_camera;
	};
}
#endif