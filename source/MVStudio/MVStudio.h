#ifndef M_MV_STUDIO_H
#define M_MV_STUDIO_H

#include "Camera.h"

class MVStudio {
public:
	static MVStudio& GetInstance();
	bool Initialise();
	bool Terminate();
	void Run();

	~MVStudio() { }

	MVStudio(const MVStudio&) = delete;
	MVStudio& operator=(const MVStudio&) = delete;
private:
	MVStudio();
	
	Mazeline::MazelineEngine* m_engine;
	Mazeline::DisplayManager* m_displayManager;
	Mazeline::InputManager* m_inputManager;
	Mazeline::GraphicsApi* m_graphicsApi;

	Mazeline::SimpleCamera* m_camera;
};

#endif