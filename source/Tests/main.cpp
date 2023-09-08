/*#include "MazelineEngine.h"
#include "Camera.h"
#include "Graphics/Shader.h"
#include "Graphics/Mesh.h"
#include "Graphics/Model.h"
#include "Graphics/VertexBuffer.h"

#include <gl/GL.h>

using namespace Mazeline;	

GraphicsApi* api;
Shader* shader;

GraphicsApi* init_display() {
	DisplayParams disp;
	disp.Width = 1080;
	disp.Height = 720;
	disp.Fullscreen = false;

	DisplayManager& display = DisplayManager::GetInstance();
	display.Create(disp);

	display.GetGraphics()->SetClearColor(0.f, 0.f, .5f, 1.f);

	return display.GetGraphics();
}

void init_shaders() {
	File vertFile("default.vert");
	File fragFile("default.frag");

	int vlen = -1;
	int flen = -1;

	vertFile.Open();
	vlen = vertFile.GetLength();

	char* vertSource = new char[vlen];
	vertFile.ReadAllText(vertSource, vlen);
	vertFile.Close();

	fragFile.Open();
	flen = fragFile.GetLength();
	char* fragSource = new char[flen];
	fragFile.ReadAllText(fragSource, flen);
	fragFile.Close();

	ShaderSource sources[2]{
		{ ShaderType::STVertex, vertSource, vlen, false, 0, 0 },
		{ ShaderType::STPixel, fragSource, flen, false, 0, 0 }
	};

	shader = new Shader(api, "default", 2, 0);
	if (!shader->Compile(2, sources)) {
	}
}

void draw_scene() {
	api->ClearColor();
	//api->DrawRectangle(-0.5f, -0.5f, 1.0f, 1.0f);
	//api->DrawRectangle(-1.f, -1.f, 1.0f, 1.0f);
}

int main(int argc, char** argv) {
	MazelineEngine& engine = MazelineEngine::GetInstance();
	DisplayManager& display = DisplayManager::GetInstance();

	if (!engine.Initialise())
		return -1;

	api = init_display();
	init_shaders();
	shader->SetActive();

	float vertices[]{
		0, 0, 0,
		0, 1, 0,
		1, 1, 0,
		1, 1, 0,
		0, 0, 0,
		1, 0, 0
	};

	float textures[]{
		0, 1,
		0, 0,
		1, 0,
		1, 0,
		0, 1,
		1, 1
	};

	VertexBuffer* buffer = new VertexBuffer(api, sizeof(vertices));
	buffer->MapData(&vertices[0], 0, sizeof(vertices));

	InputElement element;
	element.BufferId = buffer->GetHandle();
	element.BufferLocation = 0;
	element.ByteOffset = 0;
	element.Format = InputElementFormat::R32G32B32;
	element.SemanticName = "POSITION";
	element.Stride = 0;
	element.Type = InputElementType::IEVertex;

	VertexBuffer* textureBuffer = new VertexBuffer(api, sizeof(textures));
	textureBuffer->MapData(&textures[0], 0, sizeof(textures));

	InputElement texElement;
	texElement.BufferId = textureBuffer->GetHandle();
	texElement.BufferLocation = 1;
	texElement.ByteOffset = 0;
	texElement.Format = InputElementFormat::R32G32;
	texElement.SemanticName = "TEXTURE";
	texElement.Stride = 0;
	texElement.Type = InputElementType::IETextureCoord;

	VertexBuffer* bufs[] = { buffer, textureBuffer };
	InputElement elms[] = { element, texElement };
	Mesh* mesh = new Mesh(api, "quad", 2, reinterpret_cast<VertexBuffer*>(&bufs[0]), 2, &elms[0]);
	mesh->SetVertexCount(6);

	Bitmap* image = Mazeline::MazLoadImage("texture.png");
	int texId = 0;
	if (image) {
		texId = api->CreateTexture(image);
	}

	ModelPart part;
	part.Mesh = mesh;
	part.NumTextures = 1;
	part.Textures = new int[1]{ texId };

	Model model = Model(api, 1, &part);

	SimpleCamera* cam = new PerspectiveCamera(api);
	cam->SetPosition(0, 0, 3);

	int bindingBuffer = api->CreateShaderDataBinding(SDBConstant, 0, sizeof(MVP));

	InputManager& inputmgr = InputManager::GetInstance();

	while (!display.CloseRequested()) {
		cam->RecalculateBuffers();
		api->SetShaderConstantData(bindingBuffer, (void*)cam->GetCameraMVP(), 0, sizeof(MVP));
		draw_scene();
		model.Draw();
		display.Update();
		int err = glGetError();
		if (err)
			MDebugLog("Error num: 0x%06x", err);

		if (inputmgr.IsKeyDown(MKey::KeyW)) {
			cam->Move(0, 0, 0.01f);
		}

		if (inputmgr.IsKeyDown(MKey::KeyS)) {
			cam->Move(0, 0, -0.01f);
		}
	}

	api->DeleteShaderDataBinding(SDBConstant, 0, bindingBuffer);

	delete mesh;
	delete buffer;

	display.Close();

	return engine.Terminate() ? 0 : -1;
}*/

#include <ctime>
#include <iostream>

#include "Math/Matrix.h"

using namespace Mazeline;

int main(int argc, char** argv) {
	clock_t start = clock();

	Matrix4 a = Matrix4();
	Matrix4 b = Matrix4();
	Matrix4 c;

	for (int i = 0; i < 1000; i++) {
		c = a * b;
	}

	clock_t end = clock();

	std::cout << "Time taken: " << (end - start) * 1000.0 / CLOCKS_PER_SEC << "ms" << std::endl;
}