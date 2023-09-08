#ifndef M_AABB
#define M_AABB
#include "Math/Vector.h"

namespace Mazeline {
	struct AABB final {
		Vector3 Min;
		Vector3 Max;

		AABB(Vector3 min, Vector3 max);
		AABB(float minx, float miny, float minz, float maxx, float maxy, float maxz);
		bool Intersects(AABB& other);
	};
}

#endif