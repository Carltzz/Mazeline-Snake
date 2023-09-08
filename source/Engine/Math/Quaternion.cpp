#include "Quaternion.h"
#include "MathConstants.h"

using namespace Mazeline;

Quaternion Mazeline::CreateQuaternionFromEuler(float x, float y, float z) {
	using namespace std;
	double cy = cos(y * 0.5f);
	double sy = sin(y * 0.5f);
	double cp = cos(x * 0.5f);
	double sp = sin(x * 0.5f);
	double cr = cos(z * 0.5f);
	double sr = sin(z * 0.5f);
	return {
		cy * cp * cr + sy * sp * sr,
		cy * cp * sr - sy * sp * cr,
		sy * cp * cr - cy * sp * sr,
		sy * cp * sr + cy * sp * cr
	}; // Problem with this is the Z axis is inverted so to fix this
		 // Mirror the Quaternion in the XY plane by inverting the Z and W value of the quaternion
	/*return {
		cy * cp * cr - sy * sp * sr,
		cy * cp * sr - sy * sp * cr,
		sy * cp * sr + cy * sp * cr,
		sy * cp * cr + cy * sp * sr
	};*/
}

Vector3 Mazeline::CreateEulerFromQuaternion(Quaternion q) {
	float t0 = 2 * (q.W * q.X + q.Y * q.Z);
	float t1 = 1 - 2 * (q.X * q.X + q.Y * q.Y);
	float t2 = 2 * (q.W * q.Y - q.Z * q.X);
	t2 = (t2 > 1) ? 1 : t2;
	t2 = (t2 < -1) ? -1 : t2;
	float t3 = 2 * (q.W * q.Z + q.X * q.Y);
	float t4 = 1 - 2 * (q.Y * q.Y + q.Z * q.Z);
	return {
		asin(t2),
		atan2(t3, t4),
		atan2(t0, t1)
	};
}

Quaternion Mazeline::NormaliseQuaternion(Quaternion q) {
	float n = 1 / (sqrt(q.X * q.X +
		q.Y * q.Y +
		q.Z * q.Z +
		q.W * q.W));
	return {
		q.W * n,
		q.X * n,
		q.Y * n,
		q.Z * n
	};
}