#include "IndexBuffer.h"

using namespace Mazeline;

IndexBuffer::IndexBuffer(GraphicsApi* api, int size) {
	m_papi = api;
	m_handleToApi = api->CreateBuffer();
	Allocate(size);
}

void IndexBuffer::Allocate(int size, void* initialData, int offsetInBytes, int copySizeInBytes) {
	m_papi->AllocateIndexBuffer(m_handleToApi, size, initialData, offsetInBytes, copySizeInBytes);
}

void IndexBuffer::MapData(void* data, int offset, int copySize) {
	m_papi->SetIndexBufferData(m_handleToApi, data, offset, copySize);
}

int IndexBuffer::GetHandle() {
	return m_handleToApi;
}

IndexBuffer::~IndexBuffer() {
	m_papi->DeleteBuffer(m_handleToApi);
}