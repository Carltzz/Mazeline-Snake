#ifndef M_MODEL_LOADER_H
#define M_MODEL_LOADER_H

#include "File/File.h"
#include "Graphics/Model.h"

namespace Mazeline {
	enum ModelFormat {
		MFWavefrontObj
	};

	Model* MazLoadModel(const char* fileName);
}

#endif