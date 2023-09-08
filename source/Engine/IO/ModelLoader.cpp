#include "ModelLoader.h"

#include "Math/Vector.h"
#include <vector>

using namespace Mazeline;

Model* Mazeline::MazLoadModel(const char* fileName) {
	if (!FileExists(fileName))
		return nullptr;

	File file = File(fileName);

	if (file.Open(FileOpenMode::Read) != FOR_NO_ERROR)
		return nullptr;

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<Vector3> temp_vertices;
	std::vector<Vector2> temp_uvs;
	std::vector<Vector3> temp_normals;

	Model* toReturn = nullptr;
	FILE* c_file = file.GetStream();
	int result;

	Vector3 vec3Data;
	Vector2 vec2Data;

	while (true) {
		char lineHeader[128];
		result = fscanf(c_file, "%s", lineHeader);
		if (result == EOF)
			break;
		if (strcmp(lineHeader, "v") == 0) {
			fscanf(c_file, "%f %f %f\n", &vec3Data.X, &vec3Data.Y, &vec3Data.Z);
			temp_vertices.emplace_back(vec3Data.X, vec3Data.Y, vec3Data.Z);
		}
	}
}