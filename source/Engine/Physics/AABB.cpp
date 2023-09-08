#include "AABB.h"
#include <cmath>

using namespace Mazeline;

AABB::AABB(Vector3 min, Vector3 max) {
	Min.X = fminf(min.X, max.X);
	Min.Y = fminf(min.Y, max.Y);
	Min.Z = fminf(min.Z, max.Z);
	Max.X = fmaxf(min.X, max.X);
	Max.Y = fmaxf(min.Y, max.Y);
	Max.Z = fmaxf(min.Z, max.Y);
}

AABB::AABB(float minx, float miny, float minz, float maxx, float maxy, float maxz) {
	Min.X = fminf(minx, maxx);
	Min.Y = fminf(miny, maxy);
	Min.Z = fminf(minz, maxz);
	Max.X = fmaxf(minx, maxx);
	Max.Y = fmaxf(miny, maxy);
	Max.Z = fmaxf(minz, maxz);
}

bool AABB::Intersects(AABB& other) {
	return (Min.X <= other.Max.X && Max.X >= other.Min.X) &&
		 (Min.Y <= other.Max.Y && Max.Y >= other.Min.Y) &&
		 (Min.Z <= other.Max.Z && Max.Z >= other.Min.Z);
}