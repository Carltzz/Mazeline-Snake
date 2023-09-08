#include "Texture.h"

using namespace Mazeline;

Texture::Texture(GraphicsApi* api, Bitmap* bmp) {
	m_api = api;
	m_resourceId = api->CreateTexture(bmp);
	m_width = bmp->GetWidth();
	m_height = bmp->GetHeight();
}

void Texture::SetActive(int slot) {
	m_api->UseTexture(slot, m_resourceId);
}

void Texture::GetUVCoords(float* coords, int x, int y, int width, int height) {
	coords[0] = x / (float)m_width;
	coords[1] = y / (float)m_height;
	coords[2] = (x + width) / (float)m_width;
	coords[3] = (y + height) / (float)m_height;
}

Texture::~Texture() {
	m_api->DeleteTexture(m_resourceId);
}