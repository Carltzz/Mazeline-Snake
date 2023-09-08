#ifndef M_OPENGLAPI_H
#define M_OPENGLAPI_H
#include "GraphicsApi.h"

namespace Mazeline {
	class OpenGLApi final : public GraphicsApi {
	public:
		OpenGLApi();
		~OpenGLApi();

		void Initialise();

		void SetClearColor(float r, float g, float b, float a);
		void ClearScreen(FrameBufferTarget);
		void ClearColor();
		void ClearDepth();
		void ClearStencil();

		void DrawRectangle(float x, float y, float w, float h);
	
		int CreateShader(int& shaderHandle, int numSources, ShaderSource *sources);
		bool GetShaderError(char* container, int length);
		void ActivateShader(int shaderId);
		void ActivateCoreShader(DefaultShader shader);
		
		void SetShaderConstantData(int binding, void* data, int start, int copySizeInBytes);
		void SetShaderVariableData(int binding, void* data, int start, int copySizeInBytes);
		int CreateShaderDataBinding(ShaderDataBinding bindingType, int location, int sizeInBytes);
		void DeleteShaderDataBinding(ShaderDataBinding bindingType, int location, int binding);
		void DeleteShader(int shaderId);

		int CreateBuffer();
		void AllocateBuffer(int bufferId, int size, void* data = 0, int offset = 0, int copySizeInBytes = 0);
		void AllocateIndexBuffer(int bufferId, int size, void* data = 0, int offset = 0, int copySizeInBytes = 0);
		void SetBufferData(int bufferId, void* data, int offset, int copySizeInBytes);
		void SetIndexBufferData(int bufferId, void* data, int offset, int copySizeInBytes);
		void DeleteBuffer(int bufferId);

		int CreateMesh();
		void UseMeshLayout(int meshId, int numElements, InputElement* inputElements, bool update = false);
		void DeleteMesh(int meshId);

		void DrawMesh(int meshId, PrimitiveType type, int vertexCount, int offset = 0);
		void DrawIndexedMesh(int meshId, int indexId, PrimitiveType type, int indexCount, int offset = 0);
	
		int CreateTexture(Bitmap* bmp);
		void UseTexture(int slot, int textureId);
		void DeleteTexture(int textureId);
	};
};
#endif