#ifndef M_BITMAP_H
#define M_BITMAP_H

namespace Mazeline {
	enum PixelFormat {
		PF_1_Grayscale,
		PF_2_Grayscale,
		PF_4_Grayscale,
		PF_8_Grayscale,
		PF_16_Grayscale,
		PF_24_RGB,
		PF_48_RGB,
		PF_1_Indexed,
		PF_2_Indexed,
		PF_4_Indexed,
		PF_8_Indexed,
		PF_16_GrayscaleA,
		PF_32_GrayscaleA,
		PF_32_RGBA,
		PF_64_RGBA,
		PF_Invalid = 0xff
	};

	struct PngIHDR {
		int Width;
		int Height;
		int BitDepth;
		int ColorType;
		int CompressionMethod;
		int FilterMethod;
		int InterlaceMethod;
	};

	class Bitmap {
	public:
		Bitmap(int width, int height);
		Bitmap(PngIHDR header, void* scan0);
		~Bitmap();

		// Gets the width of the bitmap
		int GetWidth();

		// Gets the height of the bitmap
		int GetHeight();

		// Gets the pixel format of the bitmap
		PixelFormat GetPixelFormat();

		// Gets the pixel data of the bitmap
		char* GetPixelData();

	private:
		int m_width;
		int m_height;
		PixelFormat m_pfFormat;

		char* m_scan0;
	};

	Bitmap* MazLoadImage(const char* path);
}

#endif