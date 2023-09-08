#ifndef M_TEXTURE_H
#define M_TEXTURE_H

#include "MazResource.h"
#include "Graphics/GraphicsApi.h"

namespace Mazeline {
	class Texture : public MazResource {
	public:
		Texture(GraphicsApi* api, Bitmap* bmp);
		void SetActive(int slot);
		void GetUVCoords(float* coords, int x, int y, int width, int height);
		~Texture();
	private:
		GraphicsApi* m_api;
		int m_width;
		int m_height;
	};
}

#endif