#include "Model.h"

#include <memory>

using namespace Mazeline;

Model::Model(GraphicsApi* api, int numParts, ModelPart* parts) {
	m_papi = api;

	m_pModelParts = new ModelPart[numParts];
	memcpy(m_pModelParts, parts, sizeof(ModelPart) * numParts);
	m_partCount = numParts;
}

void Model::Draw() {
	ModelPart* part = m_pModelParts;
	for (int i = 0; i < m_partCount; i++, part++) {
		for (int j = 0; j < part->NumTextures; j++) {
			m_papi->UseTexture(j, part->Textures[j]);
		}
		part->Mesh->Draw();
	}
}

Model::~Model() {
	delete[] m_pModelParts;
}