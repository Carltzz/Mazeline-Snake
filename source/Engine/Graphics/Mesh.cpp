#include "Mesh.h"

#include <memory>

using namespace Mazeline;

Mesh::Mesh(GraphicsApi* api, char* name, int bufferCount, VertexBuffer* buffers, int numInputElements, InputElement* inputElements) {
	m_papi = api;
	m_resourceName = name;
	m_resourceId = m_papi->CreateMesh();
	
	m_buffers = new VertexBuffer*[bufferCount];
	memcpy(m_buffers, &buffers, sizeof(VertexBuffer*) * bufferCount);
	m_bufferCount = bufferCount;

	m_inputElements = new InputElement[numInputElements];
	memcpy(m_inputElements, inputElements, sizeof(InputElement) * numInputElements);
	m_inputElementCount = numInputElements;

	// TODO: Support indexing...
	m_isIndexed = false;
	m_vertexCount = 0;
	m_drawType = PrimitiveType::PTTriangles;
}

void Mesh::SetBuffer(int index, VertexBuffer* buffer) {
	m_buffers[index] = buffer;
}

VertexBuffer* Mesh::GetBuffer(int index) {
	return m_buffers[index];
}

void Mesh::SetInputElements(int numElements, InputElement* inputElements) {
	if (numElements != m_inputElementCount) {
		delete[] m_inputElements;
		m_inputElements = new InputElement[numElements];
	}
	
	memcpy(m_inputElements, inputElements, sizeof(InputElement) * numElements);
	m_inputElementCount = numElements;
}

void Mesh::SetVertexCount(int count) {
	if (count >= 0) {
		m_vertexCount = count;
	}
}

void Mesh::SetDrawType(PrimitiveType type) {
	m_drawType = type;
}

void Mesh::DeleteVertexBuffers() {
	for (int i = 0; i < m_bufferCount; i++) {
		delete m_buffers[i];
	}
	m_bufferCount = 0;
}

void Mesh::Draw() {
	m_papi->UseMeshLayout(m_resourceId, m_inputElementCount, m_inputElements);
	m_papi->DrawMesh(m_resourceId, m_drawType, m_vertexCount, 0);
}

Mesh::~Mesh() {
	delete[] m_inputElements;
	delete[] m_buffers;

	m_papi->DeleteMesh(m_resourceId);
}