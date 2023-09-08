#ifndef M_MESH_H
#define M_MESH_H

#include "MazResource.h"
#include "VertexBuffer.h"

namespace Mazeline {
	class Mesh : public MazResource {
	public:
		Mesh(GraphicsApi* api, char* name, int bufferCount, VertexBuffer* buffers, int numInputElements, InputElement* inputElements);
		~Mesh();

		// Sets the vertex buffer at the specified index
		void SetBuffer(int index, VertexBuffer* buffer);

		// Gets the vertex buffer at the specified index
		VertexBuffer* GetBuffer(int index);

		// Sets the number of vertices to draw when necessary
		void SetVertexCount(int count);

		// Sets the input layout of the model
		void SetInputElements(int numElements, InputElement* inputElements);

		// Sets the primitives drawn by the mesh
		void SetDrawType(PrimitiveType type);

		// Gets a copy of the list of the vertex buffers
		VertexBuffer* GetVertexBuffers(int& count);

		// Deletes all vertex buffers associated with the mesh
		void DeleteVertexBuffers();

		// Draws the specified model
		void Draw();

	private:
		VertexBuffer** m_buffers;
		InputElement* m_inputElements;
		bool m_isIndexed;
		int m_bufferCount;
		int m_inputElementCount;
		GraphicsApi* m_papi;
		int m_vertexCount;
		int m_drawOffset; // TODO
		PrimitiveType m_drawType;
	};
}

#endif