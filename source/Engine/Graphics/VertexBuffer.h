#ifndef M_VERTEXBUFFER_H
#define M_VERTEXBUFFER_H

#include "GraphicsApi.h"

namespace Mazeline {
	// A wrapper for vertex buffers
	class VertexBuffer {
	public:
		VertexBuffer(GraphicsApi* api, int size);
		~VertexBuffer();

		// Allocates memory to the vertex buffers
		void Allocate(int size, void* initialData = 0, int offsetInBytes = 0, int copySizeInBytes = 0);
		
		// Maps data to the vertex buffer
		void MapData(void* data, int offsetInBytes = 0, int copySizeInBytes = 0);
		
		// Gets the handle to the vertex buffer
		int GetHandle();

	private:
		int m_handleToApi;
		GraphicsApi *m_papi;
	};
}

#endif