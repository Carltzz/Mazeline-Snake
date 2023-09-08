#ifndef M_MAZELINEDISPLAY_H
#define M_MAZELINEDISPLAY_H
#include "EngineSubsystem.h"
#include "Graphics/OpenGLApi.h"
#include "Input/InputManager.h"
#include <ctime>

namespace Mazeline {
	struct DisplayParams {
		int Width;
		int Height;
		bool Fullscreen;
	};

	// A singleton class managing the display element
	class DisplayManager : public EngineSubsystem {
	public:
		// Returns the instance of the display
		static DisplayManager& GetInstance();

		// Loads the display manager
		bool Load();

		// Unloads the display manager
		bool Unload();

		// Returns the current width of the display
		int GetWidth();

		// Returns the current height of the display
		int GetHeight();

		// Creates the display
		bool Create(DisplayParams& dispParams);

		// Destroys the display
		bool Destroy();

		// Updates the display and returns the render time in milliseconds
		double Update();

		void UpdateMouse(int newX, int newY);

		// Signals for the display to be close
		void RequestClose();

		// Returns whether a request to close the display has been requested
		bool CloseRequested();

		// Forcefully closes the display
		void Close();

		// Sets the title of the display
		void SetTitle(char* titleName);

		// Gets the graphics api wrapper of the display manager
		GraphicsApi* GetGraphics();

		DisplayManager(const DisplayManager&) = delete;
		DisplayManager& operator = (const DisplayManager&) = delete;
	private:
		DisplayManager();
		~DisplayManager() { }

		DisplayParams m_displayParams;

		bool m_closeRequested;
		bool m_shouldUpdate;

		GraphicsApi* m_papi;
		InputManager* m_inputManager;

		clock_t m_lastRenderTime;
		bool m_firstFrame;
		
		int m_mouseX = -1;
		int m_mouseY = -1;
	};
}

#endif