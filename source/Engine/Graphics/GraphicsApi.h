#ifndef M_GRAPHICSAPI_H
#define M_GRAPHICSAPI_H

#include "Math/Matrix.h"
#include "Bitmap.h"

namespace Mazeline {
	enum InputElementType {
		IEVertex,
		IEColor,
		IETextureCoord,
		IENormal,
	};

	enum InputElementFormat {
		R32,
		R32G32,
		R32G32B32,
		R32G32B32A32,
		R8,
		R8G8,
		R8G8B8,
		R8G8B8A8,
	};

	struct InputElement {
		char* SemanticName; // DIRECTX 
		int BufferLocation; // OPENGL
		InputElementType Type;
		InputElementFormat Format;
		int ByteOffset;
		int Stride;
		int BufferId;
	};

	enum FrameBufferTarget {
		FBTColor = 0x01,
		FBTDepth = 0x02,
		FBTStencil = 0x04
	};

	enum ShaderType {
		STVertex = 0,
		STPixel = 1,
	};

	enum DefaultShader {
		DSVertex = 0,
		DSColoredVertex = 1,
		DSTexturedVertex = 2,
		DSColTexVertex = 3,
	};

	struct ShaderMacro {
		char* Identifier;
		char* StringValue;
	};

	struct ShaderSource {
		ShaderType Type;
		char* Data;
		int Length;
		bool IsPrecompiled;
		int NumMacros;
		ShaderMacro* Macros;
	};

	enum BufferType {
		BTVertexAttrib,
		BTIndex
	};

	enum PrimitiveType {
		PTPoints,
		PTLines,
		PTTriangles,
		PTTriangleStrips,
		PTQuads,
	};

	struct MVP {
		Matrix4 Model;
		Matrix4 View;
		Matrix4 Projection;
	};

	enum ShaderDataBinding {
		SDBConstant,
		SDBVariable
	};

	struct Vertex3pf {
		float X, Y, Z;
	};

	struct Vertex3pf2tf {
		float X, Y, Z, U, V;
	};

	struct Vertex3pf2tf4cb {
		float X, Y, Z, U, V;
		unsigned char R, G, B, A;
	};

	struct Vertex3pf4cb {
		float X, Y, Z, R, G, B, A;
	};

	struct Vertex3pf4cb2tf {
		float X, Y, Z;
		unsigned char R, G, B, A;
		float U, V;
	};

	class GraphicsApi {
	public:
		GraphicsApi() { 
			m_hasInitd = false;
			m_shaderErrLog = 0;
			m_shaderErrLogLen = 0;
		}

		// Initialises the core components of the graphics api
		virtual void Initialise() = 0;

		// Sets the color the screen should clear to when clear is called
		virtual void SetClearColor(float r, float g, float b, float a) = 0;
		
		// Clears one of the frame buffers
		virtual void ClearScreen(FrameBufferTarget) = 0;

		// Clears the color buffer of the frame
		virtual void ClearColor() = 0;

		// Clears the depth buffer of the frame
		virtual void ClearDepth() = 0;

		// Clears the stencil buffer of the frame
		virtual void ClearStencil() = 0;

		// Draws a rectangle on the screen
		virtual void DrawRectangle(float x, float y, float w, float h) = 0;

		// Creates a shader program and returns the length of the error log if an error occurs
		virtual int CreateShader(int& shaderHandle, int numSources, ShaderSource *sources) = 0;

		// Gets the last shader compilation or linkage errors (returns false if there is none)
		virtual bool GetShaderError(char* container, int length) = 0;

		// Activates the specified shader
		virtual void ActivateShader(int shaderId) = 0;

		// Activates the specified core shader
		virtual void ActivateCoreShader(DefaultShader defShader) = 0;

		// Creates a binding point allowing shader data to be written to
		virtual int CreateShaderDataBinding(ShaderDataBinding bindingType, int location, int sizeInBytes) = 0;

		// Deletes a shader data binding point
		virtual void DeleteShaderDataBinding(ShaderDataBinding bindingType, int location, int buffer) = 0;

		// Sets the shader variable at the specified location
		virtual void SetShaderConstantData(int location, void* data, int start, int copySizeInBytes) = 0;
		
		// Sets the shader variable at the specified location
		virtual void SetShaderVariableData(int location, void* data, int start, int copySizeInBytes) = 0;

		// Deletes the specified shader program
		virtual void DeleteShader(int shaderId) = 0;

		// Creates a vertex attribute or index buffer
		virtual int CreateBuffer() = 0;

		// Allocates memory to the specified buffer, destroying any initial data
		virtual void AllocateBuffer(int bufferId, int size, void* initData = 0, int offset = 0, int copySizeInBytes = 0) = 0;
		
		virtual void AllocateIndexBuffer(int bufferId, int size, void* initData = 0, int offset = 0, int copySizeInBytes = 0) = 0;

		// Maps data to the specified buffer
		virtual void SetBufferData(int bufferId, void* data, int offset, int copySizeInBytes) = 0;
		
		virtual void SetIndexBufferData(int bufferId, void* data, int offset, int copySizeInBytes) = 0;

		// Deletes a vertex or index buffer
		virtual void DeleteBuffer(int bufferId) = 0;

		// Creates a mesh
		virtual int CreateMesh() = 0;

		// Uses the input layout of the mesh
		virtual void UseMeshLayout(int meshId, int numElements, InputElement* inputElements, bool update = true) = 0;

		// Deletes the mesh
		virtual void DeleteMesh(int meshId) = 0;

		// Draws the specified mesh
		virtual void DrawMesh(int meshId, PrimitiveType type, int vertexCount, int offset = 0) = 0;

		// Draws a mesh with indices associated with it
		virtual void DrawIndexedMesh(int meshId, int indexId, PrimitiveType type, int indexCount, int offset = 0) = 0;

		// Creates a texture and returns its handle
		virtual int CreateTexture(Bitmap* bmp) = 0;

		// Sets the specified texture as the active texture
		virtual void UseTexture(int slot, int textureId) = 0;

		// Deletes a texture by its handle
		virtual void DeleteTexture(int textureId) = 0;
	protected:
		bool m_hasInitd;
		char* m_shaderErrLog;
		int m_shaderErrLogLen;
	};
};

#endif