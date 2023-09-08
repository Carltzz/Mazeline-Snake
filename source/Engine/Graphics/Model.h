#ifndef M_MODEL_H
#define M_MODEL_H

#include "Mesh.h"

namespace Mazeline {

	struct ModelPart {
		Mesh* Mesh;
		int* Textures; // TODO: Texture wrapper
		int NumTextures;
	};

	class Model {
	public:
		Model(GraphicsApi* api, int numParts, ModelPart* parts);
		Model(GraphicsApi* api);

		~Model();

		void SetPart(ModelPart* part, int index = 0);
		void DeleteModelParts();
		void Draw();
		
	private:
		ModelPart* m_pModelParts;
		int m_partCount;

		GraphicsApi* m_papi;
	};
}

#endif