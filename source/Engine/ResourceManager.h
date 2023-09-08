#ifndef M_RESOURCEMANAGER_H
#define M_RESOURCEMANAGER_H

#include "EngineSubsystem.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"

#include <vector>

namespace Mazeline {

	class ResourceManager : public EngineSubsystem {
	public:
		ResourceManager();
		~ResourceManager();

		bool Load();
		bool Unload();

		Shader* CreateBasicShader(ShaderSource vertex, ShaderSource pixel);
		Shader* CreateBasicShader(char* vertShader, char* fragShader, int numMacros = 0, void* macros = 0);
		Texture* CreateTexture(char* file);
		Texture* CreateTexture(Bitmap* bmp);
		VertexBuffer* CreateVertexBuffer(int size);

		Shader* GetShader(int id);
		Texture* GetTexture(int id);
		VertexBuffer* GetBuffer(int id);
		Model* GetModel(int id);

		bool DeleteShader(Shader* shader);
		bool DeleteTexture(Texture* texture);
		bool DeleteVertexBuffer(VertexBuffer* buffer);

	private:
		std::vector<Mesh*> m_meshs;
		std::vector<VertexBuffer*> m_vertexBuffers;
		std::vector<Shader*> m_shaders;
		std::vector<Texture*> m_textures;
		std::vector<Model*> m_models;
	};
}

#endif