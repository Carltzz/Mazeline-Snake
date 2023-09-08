#include "World.h"
#include "Graphics/GraphicsApi.h"
#include "DisplayManager.h"
#include <memory>

using namespace Mazeline;

InputElement chunkInputLayout[3]{
	{"POSITION", 0, Mazeline::InputElementType::IEVertex, Mazeline::InputElementFormat::R32G32B32, 0, sizeof(float) * 6, 0 },
	{"TEXTURE", 1, Mazeline::InputElementType::IETextureCoord, Mazeline::InputElementFormat::R32G32, sizeof(float) * 3, sizeof(float) * 6, 0 },
	{"COLOR", 2, Mazeline::InputElementType::IEColor, Mazeline::InputElementFormat::R8G8B8A8, sizeof(float) * 5, sizeof(float) * 6, 0 },
};

WorldChunk::WorldChunk(int X, int Y, int Z, int sizeX, int sizeY, int sizeZ, int sizePerElement) {
	m_position = Vector3((float)X, (float)Y, (float)Z);
	m_size = Vector3((float)sizeX, (float)sizeY, (float)sizeZ);
	m_sizePerElement = sizePerElement;
	m_chunkData = malloc((size_t)sizeX * (size_t)sizeY * (size_t)sizeZ * (size_t)sizePerElement);
	m_updateChunk = true;
	m_api = DisplayManager::GetInstance().GetGraphics();
	m_chunkVertexBuffer = new VertexBuffer(m_api, 0);
	m_chunkIndexBuffer = new IndexBuffer(m_api, 0);
	m_vertCapacity = 0;
	m_indexCapacity = 0;
	m_indexCount = 0;
	m_neighbours = 0;
	m_texture = 0;
}

void WorldChunk::Update() {
	char* data = (char*)m_chunkData;
	int id, offset;
	int vertexCount = 0, indexCount = 0;

	// Stage 1, count vertices
	for (int y = 0; y < m_size.Y; y++) {
		for (int x = 0; x < m_size.X; x++) {
			for (int z = 0; z < m_size.Z; z++) {
				offset = (x + (int)m_size.X * (y + (int)m_size.Y * z)) * m_sizePerElement;
				memcpy(&id, data + offset, m_sizePerElement);
				vertexCount += 24;
				indexCount += 36;
			}
		}
	}

	// Stage 2, generate the vertices
	Vertex3pf2tf4cb* m_chunkVerts = new Vertex3pf2tf4cb[vertexCount];
	int* m_chunkIndices = new int[indexCount];

	Vertex3pf2tf4cb* vert = &m_chunkVerts[0];
	int* index = &m_chunkIndices[0];
	int indexOffset = 0;

	for (int y = 0; y < m_size.Y; y++) {
		for (int x = 0; x < m_size.X; x++) {
			for (int z = 0; z < m_size.Z; z++) {
				// count = get_vertices(tile, x, y, z, neighbours, out vertices)
				*vert++ = { (float)x,		(float)y,		(float)z,		1, 0, 1, 1, 1, 1 };
				*vert++ = { (float)(x + 1),	(float)y,		(float)z,		1, 1, 1, 1, 1, 1 };
				*vert++ = { (float)(x + 1),	(float)(y + 1),	(float)z,		0, 1, 1, 1, 1, 1 };
				*vert++ = { (float)x,		(float)(y + 1),	(float)z,		0, 0, 1, 1, 1, 1 };
				*index++ = 0 + indexOffset; *index++ = 1 + indexOffset;	*index++ = 2 + indexOffset;
				*index++ = 2 + indexOffset; *index++ = 0 + indexOffset;	*index++ = 3 + indexOffset;
				indexOffset += 4;

				*vert++ = { (float)x,		(float)y,		(float)(z + 1),	1, 0, 1, 1, 1, 1 };
				*vert++ = { (float)(x + 1),	(float)y,		(float)(z + 1),	1, 1, 1, 1, 1, 1 };
				*vert++ = { (float)(x + 1),	(float)(y + 1),	(float)(z + 1),	0, 1, 1, 1, 1, 1 };
				*vert++ = { (float)x,		(float)(y + 1),	(float)(z + 1),	0, 0, 1, 1, 1, 1 };
				*index++ = 0 + indexOffset; *index++ = 1 + indexOffset;	*index++ = 2 + indexOffset;
				*index++ = 2 + indexOffset; *index++ = 0 + indexOffset;	*index++ = 3 + indexOffset;
				indexOffset += 4;

				*vert++ = { (float)x,		(float)y,		(float)z,		1, 0, 1, 1, 1, 1 };
				*vert++ = { (float)(x + 1),	(float)y,		(float)z,		1, 1, 1, 1, 1, 1 };
				*vert++ = { (float)(x + 1),	(float)y,		(float)(z + 1),	0, 1, 1, 1, 1, 1 };
				*vert++ = { (float)x,		(float)y,		(float)(z + 1),	0, 0, 1, 1, 1, 1 };
				*index++ = 0 + indexOffset; *index++ = 1 + indexOffset;	*index++ = 2 + indexOffset;
				*index++ = 2 + indexOffset; *index++ = 0 + indexOffset;	*index++ = 3 + indexOffset;
				indexOffset += 4;

				*vert++ = { (float)x,		(float)(y + 1),	(float)z,		1, 0, 1, 1, 1, 1 };
				*vert++ = { (float)(x + 1),	(float)(y + 1),	(float)z,		1, 1, 1, 1, 1, 1 };
				*vert++ = { (float)(x + 1),	(float)(y + 1),	(float)(z + 1),	0, 1, 1, 1, 1, 1 };
				*vert++ = { (float)x,		(float)(y + 1),	(float)(z + 1),	0, 0, 1, 1, 1, 1 };
				*index++ = 0 + indexOffset; *index++ = 1 + indexOffset;	*index++ = 2 + indexOffset;
				*index++ = 2 + indexOffset; *index++ = 0 + indexOffset;	*index++ = 3 + indexOffset;
				indexOffset += 4;

				*vert++ = { (float)x,		(float)y,		(float)z,		1, 0, 1, 1, 1, 1 };
				*vert++ = { (float)x,		(float)(y + 1),	(float)z,		1, 1, 1, 1, 1, 1 };
				*vert++ = { (float)x,		(float)(y + 1),	(float)(z + 1),	0, 1, 1, 1, 1, 1 };
				*vert++ = { (float)x,		(float)y,		(float)(z + 1),	0, 0, 1, 1, 1, 1 };
				*index++ = 0 + indexOffset; *index++ = 1 + indexOffset;	*index++ = 2 + indexOffset;
				*index++ = 2 + indexOffset; *index++ = 0 + indexOffset;	*index++ = 3 + indexOffset;
				indexOffset += 4;

				*vert++ = { (float)(x + 1),	(float)y,		(float)z,		1, 0, 1, 1, 1, 1 };
				*vert++ = { (float)(x + 1),	(float)(y + 1),	(float)z,		1, 1, 1, 1, 1, 1 };
				*vert++ = { (float)(x + 1),	(float)(y + 1),	(float)(z + 1),	0, 1, 1, 1, 1, 1 };
				*vert++ = { (float)(x + 1),	(float)y,		(float)(z + 1),	0, 0, 1, 1, 1, 1 };
				*index++ = 0 + indexOffset; *index++ = 1 + indexOffset;	*index++ = 2 + indexOffset;
				*index++ = 2 + indexOffset; *index++ = 0 + indexOffset;	*index++ = 3 + indexOffset;
				indexOffset += 4;
			}
		}
	}

	int sizeVerts = vertexCount * sizeof(Vertex3pf2tf4cb);
	int sizeIndices = indexCount * sizeof(int);
	m_indexCount = indexCount;

	if (vertexCount > m_vertCapacity) {
		m_chunkVertexBuffer->Allocate(sizeVerts, m_chunkVerts, 0, sizeVerts);
		m_vertCapacity = vertexCount;
	}
	else {
		m_chunkVertexBuffer->MapData(m_chunkVerts, 0, sizeVerts);
	}

	if (indexCount > m_indexCapacity) {
		m_chunkIndexBuffer->Allocate(sizeIndices, m_chunkIndices, 0, sizeIndices);
		m_indexCapacity = indexCount;
	} else {
		m_chunkIndexBuffer->MapData(m_chunkIndices, 0, sizeIndices);
	}

	delete[] m_chunkIndices;
	delete[] m_chunkVerts;
	m_chunkData = 0;
	m_chunkIndices = 0;
}

void WorldChunk::SetTexture(Texture* texture) {
	m_texture = texture;
}

void WorldChunk::Draw() {
	if (m_texture)
		m_texture->SetActive(0);
	chunkInputLayout[0].BufferId = m_chunkVertexBuffer->GetHandle();
	chunkInputLayout[1].BufferId = m_chunkVertexBuffer->GetHandle();
	chunkInputLayout[2].BufferId = m_chunkVertexBuffer->GetHandle();

	m_api->UseMeshLayout(0, 3, &chunkInputLayout[0]);
	m_api->DrawIndexedMesh(0, m_chunkIndexBuffer->GetHandle(), PrimitiveType::PTTriangles, m_indexCount, 0);
}

WorldChunk::~WorldChunk() {
	free(m_chunkData);
}