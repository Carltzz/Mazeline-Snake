#include "MazelineEngine.h"
#include "Camera.h"
#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Graphics/Bitmap.h"
#include "Graphics/Texture.h"
#include "Math/Vector.h"
#include "IO/File/File.h"
#include <ctime>
#include <vector>

#define TILE_SIZE 32

struct SnakePart {
	Mazeline::Vector2 Position = Mazeline::Vector2(0, 0);
	Mazeline::Matrix4 Transform = Mazeline::Mat4Identity();
	Mazeline::Mesh* Mesh = 0;
};

enum SnakePartType {
	HeadUp = 0,
	HeadDown = 1,
	HeadLeft = 2,
	HeadRight = 3,
	TailUp = 4,
	TailDown = 5,
	TailLeft = 6,
	TailRight = 7,
	Curve1 = 8,
	Curve2 = 9,
	Curve3 = 10,
	Curve4 = 11,
	BodyVertical = 12,
	BodyHorizontal = 13
};

Mazeline::Vector2 g_snakeTileLocations[14] {
	Mazeline::Vector2(3 * 64, 3 * 64), // HU
	Mazeline::Vector2(4 * 64, 2 * 64), // HD
	Mazeline::Vector2(3 * 64, 2 * 64), // HL
	Mazeline::Vector2(4 * 64, 3 * 64), // HR
	Mazeline::Vector2(4 * 64, 0 * 64), // TU
	Mazeline::Vector2(3 * 64, 1 * 64), // TD
	Mazeline::Vector2(4 * 64, 1 * 64), // TL
	Mazeline::Vector2(3 * 64, 0 * 64), // TR
	Mazeline::Vector2(0 * 64, 3 * 64), // C1
	Mazeline::Vector2(2 * 64, 3 * 64), // C2
	Mazeline::Vector2(0 * 64, 2 * 64), // C3
	Mazeline::Vector2(2 * 64, 1 * 64), // C4
	Mazeline::Vector2(2 * 64, 2 * 64), // BV
	Mazeline::Vector2(1 * 64, 3 * 64) // BH
};

struct Snake {
	SnakePart SnakeHead;
	std::vector<SnakePart> SnakeBody;
};

Mazeline::MazelineEngine* g_engine;
Mazeline::DisplayManager* g_displayManager;
Mazeline::InputManager* g_inputManager;
Mazeline::OrthogonalCamera* g_camera;
Mazeline::GraphicsApi* g_api;

Mazeline::Shader* g_shader;
int g_cameraBufferBinding;
Mazeline::Model* g_snakeModel;
Mazeline::ModelPart* g_snakeHead;
Mazeline::ModelPart* g_snakeBody;
Mazeline::Texture* g_snakeTexture;
Mazeline::Vector2 g_snakeVelocity = Mazeline::Vector2(-1, 0);

Mazeline::InputElement g_inputLayout[3]{
	{"POSITION", 0, Mazeline::InputElementType::IEVertex, Mazeline::InputElementFormat::R32G32B32, 0, sizeof(float) * 8, 0 },
	{"TEXTURE", 1, Mazeline::InputElementType::IETextureCoord, Mazeline::InputElementFormat::R32G32, sizeof(float) * 3, sizeof(float) * 8, 0 },
	{"COLOR", 2, Mazeline::InputElementType::IEColor, Mazeline::InputElementFormat::R32G32B32, sizeof(float) * 5, sizeof(float) * 8, 0 },
};

float g_snakeVertexBuffer[8 * 6]{
	0,	0,	0,	 0, 1,	 1, 1, 1,
	0,	TILE_SIZE, 0,	 0, 0,	 1, 1, 1,
	TILE_SIZE, TILE_SIZE, 0,	 1, 0,	 1, 1, 1,
	TILE_SIZE, TILE_SIZE, 0,	 1, 0,	 1, 1, 1,
	0,	0,	0,	 0, 1,	 1, 1, 1,
	TILE_SIZE, 0,	0,	 1, 1,	 1, 1, 1,
};

void set_vertexbuffer_uvs(float sx, float sy, float ex, float ey) {
	g_snakeVertexBuffer[3] = sx; g_snakeVertexBuffer[4] = ey;
	g_snakeVertexBuffer[11] = sx; g_snakeVertexBuffer[12] = sy;
	g_snakeVertexBuffer[19] = ex; g_snakeVertexBuffer[20] = sy;
	g_snakeVertexBuffer[27] = ex; g_snakeVertexBuffer[28] = sy;
	g_snakeVertexBuffer[35] = sx; g_snakeVertexBuffer[36] = ey;
	g_snakeVertexBuffer[43] = ex; g_snakeVertexBuffer[44] = ey;
}

Snake g_snake;

void create_display() {
	Mazeline::DisplayParams displayParams;
	displayParams.Fullscreen = false;
	displayParams.Width = 600;
	displayParams.Height = 600;
	g_displayManager = &Mazeline::DisplayManager::GetInstance();
	g_displayManager->Create(displayParams);
	g_api = g_displayManager->GetGraphics();
}

void getShaderSource(char* file, int& length, char** src) {
	Mazeline::File shaderFile(file);
	shaderFile.Open();
	length = shaderFile.GetLength();
	*src = new char[length];
	shaderFile.ReadAllText(*src, length);
	shaderFile.Close();
}

void init_shader() {
	char* vertSource; int vertSourceLen;
	char* fragSource; int fragSourceLen;
	getShaderSource("res/snake_shader.vert", vertSourceLen, &vertSource);
	getShaderSource("res/snake_shader.frag", fragSourceLen, &fragSource);

	Mazeline::ShaderSource vertexShader;
	vertexShader.Data = vertSource;
	vertexShader.IsPrecompiled = false;
	vertexShader.Length = vertSourceLen;
	vertexShader.Macros = 0;
	vertexShader.NumMacros = 0;
	vertexShader.Type = Mazeline::ShaderType::STVertex;

	Mazeline::ShaderSource fragShader;
	fragShader.Data = fragSource;
	fragShader.IsPrecompiled = false;
	fragShader.Length = fragSourceLen;
	fragShader.Macros = 0;
	fragShader.NumMacros = 0;
	fragShader.Type = Mazeline::ShaderType::STPixel;

	Mazeline::ShaderSource sources[2]{
		vertexShader,
		fragShader
	};

	g_shader = new Mazeline::Shader(g_api, "default", 2, 0);
	g_shader->Compile(2, sources);
}

void init_textures() {
	Mazeline::Bitmap* texture = Mazeline::MazLoadImage("res/snake.png");
	if (texture)
		g_snakeTexture = new Mazeline::Texture(g_api, texture);
	delete texture;
}

void setup_camera() {
	g_camera = new Mazeline::OrthogonalCamera(g_api, 600, 600);
}

void cleanup() {
	g_snake.SnakeHead.Mesh->DeleteVertexBuffers();
	delete g_snake.SnakeHead.Mesh;

	for (std::vector<SnakePart>::iterator it = g_snake.SnakeBody.begin(); it != g_snake.SnakeBody.end(); ++it) {
		(*it).Mesh->DeleteVertexBuffers();
		delete (*it).Mesh;
	}

	delete g_camera;
	delete g_snakeTexture;
	delete g_shader;
}

void draw_snake() {
	g_snakeTexture->SetActive(0);

	g_api->SetShaderConstantData(
		g_cameraBufferBinding, (void*)&g_snake.SnakeHead.Transform,
		0, sizeof(Mazeline::Matrix4)
	);

	g_snake.SnakeHead.Mesh->Draw();

	for (std::vector<SnakePart>::iterator it = g_snake.SnakeBody.begin();
		it != g_snake.SnakeBody.end(); ++it) {
		g_api->SetShaderConstantData(
			g_cameraBufferBinding, (void*)&(*it).Transform,
			0, sizeof(Mazeline::Matrix4)
		);
		(*it).Mesh->Draw();
	}
}

void get_snakepart_uvs() {
	Mazeline::Vector2 snakeHeadTilePos = Mazeline::Vector2(3, 1);

	if (g_snakeVelocity.X > 0) {
		snakeHeadTilePos = g_snakeTileLocations[HeadRight];
	}
	else if (g_snakeVelocity.X < 0) {
		snakeHeadTilePos = g_snakeTileLocations[HeadLeft];
	}
	else if (g_snakeVelocity.Y < 0) {
		snakeHeadTilePos = g_snakeTileLocations[HeadUp];
	}
	else if (g_snakeVelocity.Y > 0) {
		snakeHeadTilePos = g_snakeTileLocations[HeadDown];
	}

	float headCoords[4];
	g_snakeTexture->GetUVCoords(headCoords, snakeHeadTilePos.X, snakeHeadTilePos.Y, 64, 64);
	set_vertexbuffer_uvs(headCoords[0], headCoords[1], headCoords[2], headCoords[3]);
	g_snake.SnakeHead.Mesh->GetBuffer(0)->MapData(g_snakeVertexBuffer, 0, sizeof(g_snakeVertexBuffer));

	std::vector<SnakePart>::iterator it, next;
	SnakePart* prev = &g_snake.SnakeHead;
	SnakePart* current = 0;
	bool isTail = false;
	for (it = g_snake.SnakeBody.begin(); it != g_snake.SnakeBody.end(); ++it) {
		next = it + 1;
		if (next == g_snake.SnakeBody.end())
			isTail = true;
		current = &*it;
		int pdx = current->Position.X - prev->Position.X;
		int pdy = current->Position.Y - prev->Position.Y;
		float coords[4];
		Mazeline::Vector2 tilePos = Mazeline::Vector2(0, 0);

		if (isTail) {
			if (pdx == 0) {
				tilePos = g_snakeTileLocations[pdy > 0 ? TailDown : TailUp];

			}
			if (pdy == 0) {
				tilePos = g_snakeTileLocations[pdx > 0 ? TailRight : TailLeft];

			}
		}
		else {
			int ndx = current->Position.X - (*next).Position.X;
			int ndy = current->Position.Y - (*next).Position.Y;

			if (ndx == 0)
				tilePos = g_snakeTileLocations[BodyVertical];
			else if (ndy == 0)
				tilePos = g_snakeTileLocations[BodyHorizontal];

			if ((ndx < 0 && pdy < 0) || (ndy < 0 && pdx < 0)) {
				tilePos = g_snakeTileLocations[Curve1];
			}
			else if ((ndx > 0 && pdy < 0) || (ndy < 0 && pdx > 0)) {
				tilePos = g_snakeTileLocations[Curve2];
			}
			else if ((ndx < 0 && pdy > 0) || (ndy > 0 && pdx < 0)) {
				tilePos = g_snakeTileLocations[Curve3];
			}
			else if ((ndx > 0 && pdy > 0) || (ndy > 0 && pdx > 0)) {
				tilePos = g_snakeTileLocations[Curve4];
			}
		}

		g_snakeTexture->GetUVCoords(coords, tilePos.X, tilePos.Y, 64, 64);
		set_vertexbuffer_uvs(coords[0], coords[1], coords[2], coords[3]);
		Mazeline::VertexBuffer* buffer = current->Mesh->GetBuffer(0);
		buffer->MapData(g_snakeVertexBuffer, 0, sizeof(g_snakeVertexBuffer));
		prev = current;
	}
}

void draw_scene() {
	g_api->ClearColor();
	g_api->ClearDepth();

	draw_snake();
}

void move_snake() {
	Mazeline::Vector2 oldPos = g_snake.SnakeHead.Position;
	Mazeline::Vector2 temp = Mazeline::Vector2(0, 0);
	g_snake.SnakeHead.Position += g_snakeVelocity;
	g_snake.SnakeHead.Transform =
		Mazeline::CreateTranslationMatrix(
			Mazeline::Vector3(
				g_snake.SnakeHead.Position.X * TILE_SIZE,
				g_snake.SnakeHead.Position.Y * TILE_SIZE,
				0
			)
		);
	auto it = g_snake.SnakeBody.begin();
	for (; it != g_snake.SnakeBody.end(); ++it) {
		temp = oldPos;
		oldPos = (*it).Position;
		(*it).Position = temp;
		(*it).Transform =
			Mazeline::CreateTranslationMatrix(
				Mazeline::Vector3(
				(*it).Position.X * TILE_SIZE,
					(*it).Position.Y * TILE_SIZE,
					0
				)
			);
	}

	get_snakepart_uvs();
}

void do_logic() {
	move_snake();
}

void add_body_part(int x, int y) {
	SnakePart bodyPart;
	bodyPart.Position.X = x;
	bodyPart.Position.Y = y;
	bodyPart.Transform = 
		Mazeline::CreateTranslationMatrix(
			Mazeline::Vector3(
				bodyPart.Position.X * TILE_SIZE, 
				bodyPart.Position.Y * TILE_SIZE, 
				0
			)
		);
	Mazeline::VertexBuffer* buffer
		= new Mazeline::VertexBuffer(g_api, 8 * 6 * sizeof(float));
	buffer->MapData(g_snakeVertexBuffer, 0, 8 * 6 * sizeof(float));
	for (int i = 0; i < 3; i++)
		g_inputLayout[i].BufferId = buffer->GetHandle();
	bodyPart.Mesh = new Mazeline::Mesh(
		g_api,
		"snake_body",
		1,
		buffer,
		3,
		g_inputLayout
	);
	bodyPart.Mesh->SetVertexCount(6);
	bodyPart.Mesh->SetDrawType(Mazeline::PrimitiveType::PTTriangles);
	g_snake.SnakeBody.insert(g_snake.SnakeBody.end(), bodyPart);
}

void load_snake_sprite() {
	Mazeline::VertexBuffer* buffer 
		= new Mazeline::VertexBuffer(g_api, 8 * 6 * sizeof(float));
	buffer->MapData(g_snakeVertexBuffer, 0, 8 * 6 * sizeof(float));
	for (int i = 0; i < 3; i++)
		g_inputLayout[i].BufferId = buffer->GetHandle();
	g_snake.SnakeHead.Position = Mazeline::Vector2(15, 15);

	g_snake.SnakeHead.Transform = Mazeline::CreateTranslationMatrix(
		Mazeline::Vector3(
			g_snake.SnakeHead.Position.X * TILE_SIZE,
			g_snake.SnakeHead.Position.Y * TILE_SIZE,
			0
		)
	);

	g_snake.SnakeHead.Mesh = new Mazeline::Mesh(
		g_api,
		"snake_head",
		1,
		buffer,
		3,
		g_inputLayout
	);

	g_snake.SnakeHead.Mesh->SetVertexCount(6);
	g_snake.SnakeHead.Mesh->SetDrawType(Mazeline::PrimitiveType::PTTriangles);

	get_snakepart_uvs();
}

void init_game() {
	load_snake_sprite();
	add_body_part(g_snake.SnakeHead.Position.X + 1, g_snake.SnakeHead.Position.Y);
	add_body_part(g_snake.SnakeHead.Position.X + 2, g_snake.SnakeHead.Position.Y);
	add_body_part(g_snake.SnakeHead.Position.X + 3, g_snake.SnakeHead.Position.Y);
}

int main(int argc, char** argv) {
	g_engine = &Mazeline::MazelineEngine::GetInstance();

	if (!g_engine->Initialise())
		return -1;

	create_display();
	setup_camera();

	init_shader();
	init_textures();
	init_game();
	load_snake_sprite();

	g_camera->RecalculateBuffers();
	g_shader->SetActive();
	g_cameraBufferBinding = g_api->CreateShaderDataBinding(Mazeline::ShaderDataBinding::SDBConstant, 0, sizeof(Mazeline::MVP));

	g_api->SetClearColor(0x99 / 255.f, 0xcc / 255.f, 1, 1);

	double accumulator = 0;
	clock_t renderTime, renderTimeEnd;

	while (!g_displayManager->CloseRequested()) {
		renderTime = clock();
		draw_scene();
		g_api->SetShaderConstantData(g_cameraBufferBinding, (void*)g_camera->GetCameraMVP(), 0, sizeof(Mazeline::MVP));
		
		if (g_inputManager->IsKeyDown(Mazeline::KeyUp) && g_snakeVelocity.Y >= 0) {
			g_snakeVelocity = Mazeline::Vector2(0, -1);
		}if (g_inputManager->IsKeyDown(Mazeline::KeyDown) && g_snakeVelocity.Y <= 0) {
			g_snakeVelocity = Mazeline::Vector2(0, 1);
		}if (g_inputManager->IsKeyDown(Mazeline::KeyLeft) && g_snakeVelocity.X <= 0) {
			g_snakeVelocity = Mazeline::Vector2(-1, 0);
		}if (g_inputManager->IsKeyDown(Mazeline::KeyRight) && g_snakeVelocity.X >= 0) {
			g_snakeVelocity = Mazeline::Vector2(1, 0);
		}

		while (accumulator >= 0.25) {
			do_logic();
			accumulator -= 0.25;
		}
		g_displayManager->Update();
		renderTimeEnd = clock();
		accumulator += (double)(renderTimeEnd - renderTime) / CLOCKS_PER_SEC;
	}
	
	cleanup();

	return g_engine->Terminate();
}