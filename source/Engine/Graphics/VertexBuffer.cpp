#include "VertexBuffer.h"

using namespace Mazeline;

VertexBuffer::VertexBuffer(GraphicsApi* api, int size) {
	m_papi = api;
	m_handleToApi = api->CreateBuffer();
	Allocate(size);
}

void VertexBuffer::Allocate(int size, void* initialData, int offsetInBytes, int copySizeInBytes) {
	m_papi->AllocateBuffer(m_handleToApi, size, initialData, offsetInBytes, copySizeInBytes);
}

void VertexBuffer::MapData(void* data, int offset, int copySize) {
	m_papi->SetBufferData(m_handleToApi, data, offset, copySize);
}

int VertexBuffer::GetHandle() {
	return m_handleToApi;
}

VertexBuffer::~VertexBuffer() {
	m_papi->DeleteBuffer(m_handleToApi);
}