#ifndef M_INDEXBUFFER_H
#define M_INDEXBUFFER_H

#include "GraphicsApi.h"

namespace Mazeline {
	// A wrapper for index buffers
	class IndexBuffer {
	public:
		IndexBuffer(GraphicsApi* api, int size);
		~IndexBuffer();

		// Allocates memory to the index buffers
		void Allocate(int size, void* initialData = 0, int offsetInBytes = 0, int copySizeInBytes = 0);

		// Maps data to the index buffer
		void MapData(void* data, int offsetInBytes = 0, int copySizeInBytes = 0);

		// Gets the handle to the index buffer
		int GetHandle();

	private:
		int m_handleToApi;
		GraphicsApi *m_papi;
	};
}

#endif