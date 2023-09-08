#include "MazelineEngine.h"
#include "MVStudio.h"
#include "Graphics/Model.h"
#include "World/World.h"
#include "IO/ModelLoader.h"
#include <iostream>

using namespace Mazeline;

WorldChunk* chunk;
Model* theniggastarfish;

MVStudio::MVStudio() {
	m_graphicsApi = 0;
	m_engine = 0;
	m_displayManager = 0;
	m_inputManager = 0;
	m_camera = 0;
}

MVStudio& MVStudio::GetInstance() {
	static MVStudio instance;
	return instance;
}

bool MVStudio::Initialise() {
	bool noError = true;

	// Initialise the engine
	m_engine = &MazelineEngine::GetInstance();
	noError |= m_engine->Initialise();

	// Initialise the display
	m_displayManager = &DisplayManager::GetInstance();
	DisplayParams display;
	display.Fullscreen = false;
	display.Width = 1280;
	display.Height = 720;
	noError |= m_displayManager->Create(display);

	// Initialise the graphics
	m_graphicsApi = m_displayManager->GetGraphics();
	noError |= (m_graphicsApi != 0);

	m_inputManager = &InputManager::GetInstance();

	return noError;
}

bool MVStudio::Terminate() {
	bool noError = true;

	// Destroy the display
	noError |= m_displayManager->Destroy();

	// Terminate the engine
	noError |= m_engine->Terminate();

	return noError;
}

void MVStudio::Run() {
	bool closeRequested = false;
	double renderTime = 0;

	m_camera = new PerspectiveCamera(m_graphicsApi);

	m_camera->SetPosition(0, 0, 0);
	m_camera->SetNearPlane(0.1f);
	m_camera->SetFarPlane(1000.0f);
	m_camera->SetRotation(0, 0 * DEG2RAD, 0);
	((PerspectiveCamera*)m_camera)->SetFieldOfView(90.0f);
	((PerspectiveCamera*)m_camera)->SetAspectRatio(1280.0f / 720.0f);
	((PerspectiveCamera*)m_camera)->SetInputMode(*m_inputManager, true, true);

	// m_engine->SetCamera(m_camera);

	chunk = new WorldChunk(0, 0, 0, 16, 16, 16, 1);
	chunk->Update();

	m_graphicsApi->ActivateCoreShader(DefaultShader::DSColTexVertex);
	
	Bitmap* bmp = MazLoadImage("texture.png");
	Texture* texture = new Texture(m_graphicsApi, bmp);
	
	delete bmp;

	chunk->SetTexture(texture);

	int bindingLocation = m_graphicsApi->CreateShaderDataBinding(SDBConstant, 0, sizeof(MVP));

	theniggastarfish = MazLoadModel("theniggastarfishobj.obj");

	while (true) {
		closeRequested = m_displayManager->CloseRequested();

		if (closeRequested) {
			// Handle exit event
			closeRequested = false;
			break;
		}
		else {
			// Logic
			static_cast<PerspectiveCamera*>(m_camera)->HandleInput(*m_inputManager);

			if (m_camera->RecalculateBuffers())
				m_graphicsApi->SetShaderConstantData(bindingLocation, (void*)m_camera->GetCameraMVP(), 0, sizeof(MVP));
			// Render
			m_graphicsApi->ClearColor();
			chunk->Draw();
			theniggastarfish->Draw();
			renderTime = m_displayManager->Update();
		}
	}

	delete m_camera;
	delete chunk;
}

int main(int argc, char** argv) {
	MVStudio& studio = MVStudio::GetInstance();

	if (!studio.Initialise()) {
		std::cout << "There was an error when initialising." << std::endl;
		return -1;
	}

	studio.Run();

	if (!studio.Terminate()) {
		std::cout << "There was an error when terminating the studio." << std::endl;
		return -1;
	}

	return 0;
}