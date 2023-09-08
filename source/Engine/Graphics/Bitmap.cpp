#include "Bitmap.h"
#include "libpng/png.h"
#include <memory>

using namespace Mazeline;

Bitmap::Bitmap(int w, int h) {
	m_scan0 = new char[w * h * 32];
	m_pfFormat = PixelFormat::PF_32_RGBA;
	m_width = w;
	m_height = h;
}

Bitmap::Bitmap(PngIHDR pngHeader, void* data) {
	m_width = pngHeader.Width;
	m_height = pngHeader.Height;
	m_pfFormat = PixelFormat::PF_Invalid;
	m_scan0 = reinterpret_cast<char*>(data);

	if (pngHeader.ColorType == 0) { // Grayscale
		switch (pngHeader.BitDepth) {
		case 1: m_pfFormat = PixelFormat::PF_1_Grayscale; break;
		case 2: m_pfFormat = PixelFormat::PF_2_Grayscale; break;
		case 4: m_pfFormat = PixelFormat::PF_4_Grayscale; break;
		case 8: m_pfFormat = PixelFormat::PF_8_Grayscale; break;
		case 16: m_pfFormat = PixelFormat::PF_16_Grayscale; break;
		}
	}
	else if (pngHeader.ColorType == 2) { // RGB
		switch (pngHeader.BitDepth) {
		case 8: m_pfFormat = PixelFormat::PF_24_RGB; break;
		case 16: m_pfFormat = PixelFormat::PF_48_RGB; break;
		}
	}
	else if (pngHeader.ColorType == 3) {
		switch (pngHeader.BitDepth) { // Indexed palette
		case 1: m_pfFormat = PixelFormat::PF_1_Indexed; break;
		case 2: m_pfFormat = PixelFormat::PF_2_Indexed; break;
		case 4: m_pfFormat = PixelFormat::PF_4_Indexed; break;
		case 8: m_pfFormat = PixelFormat::PF_8_Indexed; break;
		}
	}
	else if (pngHeader.ColorType == 4) { // Grayscale with alpha
		switch (pngHeader.BitDepth) {
		case 8: m_pfFormat = PixelFormat::PF_16_GrayscaleA; break;
		case 16: m_pfFormat = PixelFormat::PF_32_GrayscaleA; break;
		}
	}
	else if (pngHeader.ColorType == 6) {
		switch (pngHeader.BitDepth) {
		case 8: m_pfFormat = PixelFormat::PF_32_RGBA; break;
		case 16: m_pfFormat = PixelFormat::PF_64_RGBA; break;
		}
	}
}

int Bitmap::GetWidth() {
	return m_width;
}

int Bitmap::GetHeight() {
	return m_height;
}

PixelFormat Bitmap::GetPixelFormat() {
	return m_pfFormat;
}

char* Bitmap::GetPixelData() {
	return m_scan0;
}

Bitmap* Mazeline::MazLoadImage(const char* path) {
	png_structp png_ptr;
	png_infop info_ptr;
	unsigned int sig_read;
	FILE* fp;

	if ((fp = fopen(path, "rb")) == 0)
		return 0;

	png_ptr = png_create_read_struct(
		PNG_LIBPNG_VER_STRING,
		0, 0, 0
	);

	if (!png_ptr) {
		fclose(fp);
		return 0;
	}

	info_ptr = png_create_info_struct(png_ptr);
	
	if (!info_ptr) {
		fclose(fp);
		png_destroy_read_struct(&png_ptr, 0, 0);
		return 0;
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		fclose(fp);
		return false;
	}

	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, sig_read);

	png_read_png(png_ptr, info_ptr,
		PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND,
		0);

	PngIHDR header;

	png_get_IHDR(png_ptr, info_ptr,
		reinterpret_cast<unsigned int*>(&header.Width),
		reinterpret_cast<unsigned int*>(&header.Height),
		&header.BitDepth, &header.ColorType, &header.InterlaceMethod,
		&header.CompressionMethod, &header.FilterMethod);

	unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
	char* data = new char[row_bytes * header.Height];

	png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

	for (int i = 0; i < header.Height; i++) {
		memcpy(data + (row_bytes * (header.Height - 1 - i)), row_pointers[i], row_bytes);
	}

	png_destroy_read_struct(&png_ptr, &info_ptr, 0);
	fclose(fp);

	return new Bitmap(header, data);
}

Bitmap::~Bitmap() {
	delete[] m_scan0;
}