#ifndef MV_WORLD_H
#define MV_WORLD_H

#define MV_DEFAULT_CHUNK_X 16
#define MV_DEFAULT_CHUNK_Y 16
#define MV_DEFAULT_CHUNK_Z 16

#include "Math/Vector.h"
#include "Graphics/GraphicsApi.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/Texture.h"
#include "Physics/AABB.h"

enum ChunkNeighbour {
	PositiveX = 0,
	PositiveY = 1,
	PositiveZ = 2,
	NegativeX = 3,
	NegativeY = 4,
	NegativeZ = 5
};

class WorldChunk {
public:
	WorldChunk(int X, int Y, int Z, int sizeX, int sizeY, int sizeZ, int sizePerElement);

	void Update();
	void SetTexture(Mazeline::Texture* texture);
	void Draw();

	~WorldChunk();
private:
	Mazeline::Vector3 m_position;
	Mazeline::Vector3 m_size;
	int m_sizePerElement;
	bool m_updateChunk;
	void* m_chunkData;
	int m_vertCapacity;
	int m_indexCapacity;
	int m_indexCount;
	Mazeline::VertexBuffer* m_chunkVertexBuffer;
	Mazeline::IndexBuffer* m_chunkIndexBuffer;
	Mazeline::GraphicsApi* m_api;
	WorldChunk* m_neighbours;
	Mazeline::Texture* m_texture;
};

class World {
public:
	World();
private:
	Mazeline::AABB m_maxGenerationBounds;
	Mazeline::AABB m_maxWorldBounds;
	Mazeline::Vector3 m_chunkSize;
};

#endif