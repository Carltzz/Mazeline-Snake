#ifndef M_QUATERNION_H
#define M_QUATERNION_H
#include "Vector.h"
#include <cmath>

namespace Mazeline {
	struct Quaternion {
		double W, X, Y, Z;
	};

	Quaternion CreateQuaternionFromEuler(float x, float y, float z);
	Vector3 CreateEulerFromQuaternion(Quaternion q);

	Quaternion NormaliseQuaternion(Quaternion q);
}
#endif