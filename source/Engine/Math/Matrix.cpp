#include "Matrix.h"
#include "Quaternion.h"
#include "Vector.h"

using namespace Mazeline;

Matrix4 Mazeline::CreatePerspectiveMatrix(float fov, float aspect, float zNear, float zFar) {
	const float rTanFov = 1 / std::tan(fov * 0.5);
	return {
		rTanFov / aspect, 0.0f, 0.0f, 0.0f,
		0.0f, rTanFov, 0.0f, 0.0f,
		0.0f, 0.0f, (zFar + zNear) / (zNear - zFar), (2.0f * zFar * zNear) / (zNear - zFar),
		0.0f, 0.0f, -1.0f, 0.0f
	};
}

Matrix4 Mazeline::CreateOrthographicMatrix(int width, int height, float zNear, float zFar) {
	/*return {
		2.0f / width, 0.0f, 0.0f, 0.0f,
		0.0f, -2.0f / height, 0.0f, 0.0f,
		0.0f, 0.0f, 2.0f / (zNear - zFar), 0.0f,
		-1.0f, 1.0f, (zFar + zNear) / (zNear - zFar), 1.0f
	};*/
	return {
		2.0f / width, 0.0f, 0.0f, 0.0f,
		0.0f, -2.0f / height, 0.0f, 0.0f,
		0.0f, 0.0f, 2.0f / (zNear - zFar), 0.0f,
		-1.0f, 1.0f, -(zFar + zNear) / (zFar - zNear), 1.0f
	};
}

Matrix4 Mazeline::CreateTranslationMatrix(Vector3 translation) {
	return Matrix4{
		1.0f, 0.0f, 0.0f, translation.X,
		0.0f, 1.0f, 0.0f, translation.Y,
		0.0f, 0.0f, 1.0f, translation.Z,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}

Matrix4 Mazeline::CreateScaleMatrix(Vector3 scale) {
	return Matrix4{
		scale.X, 0, 0, 0,
		0, scale.Y, 0, 0,
		0, 0, scale.Z, 0,
		0, 0, 0, 1
	};
}

Matrix4	Mazeline::CreateFromQuaternion(Quaternion q) {
	q = NormaliseQuaternion(q);
	return {
		(float)(1.0f - 2.0f * q.Z * q.Z - 2.0f * q.Y * q.Y),
		(float)(-2.0f * q.X * q.Y - 2.0f * q.Z * q.W),
		(float)(-2.0f * q.X * q.Z + 2.0f * q.Y * q.W),
		0.0f,
		(float)(-2.0f * q.X * q.Y + 2.0f * q.Z * q.W),
		(float)(1.0f - 2.0f * q.X * q.X - 2.0f * q.Z * q.Z),
		(float)(2.0f * q.Y * q.Z + 2.0f * q.X * q.W),
		0.0f,
		(float)(2.0f * q.X * q.Z + 2.0f * q.Y * q.W),
		(float)(-2.0f * q.Y * q.Z + 2.0f * q.X * q.W),
		(float)(1.0f - 2.0f * q.X * q.X - 2.0f * q.Y * q.Y),
		0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}

/*Matrix4 Mazeline::CreateRotation(Vector3 euler) {
	return CreateFromQuaternion(CreateQuaternionFromEuler(euler.X, euler.Y, euler.Z));
}*/

Matrix4 Mazeline::CreateEulerRotationXYZ(Vector3 euler) {
	return
		CreateEulerRotationZ(euler.Z) *
		CreateEulerRotationY(euler.Y) *
		CreateEulerRotationX(euler.X);
}

Matrix4 Mazeline::CreateEulerRotationXZY(Vector3 euler) {
	return
		CreateEulerRotationY(euler.Y) *
		CreateEulerRotationZ(euler.Z) *
		CreateEulerRotationX(euler.X);
}

Matrix4 Mazeline::CreateEulerRotationYXZ(Vector3 euler) {
	return
		CreateEulerRotationZ(euler.Z) *
		CreateEulerRotationX(euler.X) *
		CreateEulerRotationY(euler.Y);
}

Matrix4 Mazeline::CreateEulerRotationYZX(Vector3 euler) {
	return
		CreateEulerRotationX(euler.X) *
		CreateEulerRotationZ(euler.Z) *
		CreateEulerRotationY(euler.Y);
}

Matrix4 Mazeline::CreateEulerRotationZYX(Vector3 euler) {
	return
		CreateEulerRotationX(euler.X) *
		CreateEulerRotationY(euler.Y) *
		CreateEulerRotationZ(euler.Z);
}

Matrix4 Mazeline::CreateEulerRotationZXY(Vector3 euler) {
	return
		CreateEulerRotationY(euler.Y) *
		CreateEulerRotationX(euler.X) *
		CreateEulerRotationZ(euler.Z);
}

Matrix4 Mazeline::CreateEulerRotationXY(float X, float Y) {
	return
		CreateEulerRotationY(Y) *
		CreateEulerRotationX(X);
}

Matrix4 Mazeline::CreateEulerRotationYX(float X, float Y) {
	return
		CreateEulerRotationX(X) *
		CreateEulerRotationY(Y);
}

Matrix4 Mazeline::CreateEulerRotationXZ(float X, float Z) {
	return
		CreateEulerRotationZ(Z) *
		CreateEulerRotationX(X);
}

Matrix4 Mazeline::CreateEulerRotationZX(float X, float Z) {
	return
		CreateEulerRotationX(X) *
		CreateEulerRotationZ(Z);
}

Matrix4 Mazeline::CreateEulerRotationYZ(float Y, float Z) {
	return
		CreateEulerRotationZ(Z) *
		CreateEulerRotationY(Y);
}

Matrix4 Mazeline::CreateEulerRotationZY(float Y, float Z) {
	return
		CreateEulerRotationY(Y) *
		CreateEulerRotationZ(Z);
}

Matrix4 Mazeline::CreateEulerRotationX(float X) {
	return {
		1, 0, 0, 0,
		0, cos(X), -sin(X), 0,
		0, sin(X), cos(X), 0,
		0, 0, 0, 1
	};
}

Matrix4 Mazeline::CreateEulerRotationY(float Y) {
	return {
		cos(Y), 0, sin(Y), 0,
		0, 1, 0, 0,
		-sin(Y), 0, cos(Y), 0,
		0, 0, 0, 1
	};
}

Matrix4 Mazeline::CreateEulerRotationZ(float Z) {
	return {
		cos(Z), -sin(Z), 0, 0,
		sin(Z), cos(Z), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
}

Matrix4 Mazeline::CreateViewMatrix(Vector3 pos, Vector3 rot, Vector3 scale = { 1, 1, 1 }) {
	return CreateTranslationMatrix(-pos) *
		CreateEulerRotationYXZ(rot) *
		CreateScaleMatrix(scale);
}

Matrix4 Mazeline::MultiplyMat4(Matrix4& l, Matrix4& r) {
	return {
		l.m00 * r.m00 + l.m01 * r.m10 + l.m02 * r.m20 + l.m03 * r.m30,
		l.m00 * r.m01 + l.m01 * r.m11 + l.m02 * r.m21 + l.m03 * r.m31,
		l.m00 * r.m02 + l.m01 * r.m12 + l.m02 * r.m22 + l.m03 * r.m32,
		l.m00 * r.m03 + l.m01 * r.m13 + l.m02 * r.m23 + l.m03 * r.m33,
		l.m10 * r.m00 + l.m11 * r.m10 + l.m12 * r.m20 + l.m13 * r.m30,
		l.m10 * r.m01 + l.m11 * r.m11 + l.m12 * r.m21 + l.m13 * r.m31,
		l.m10 * r.m02 + l.m11 * r.m12 + l.m12 * r.m22 + l.m13 * r.m32,
		l.m10 * r.m03 + l.m11 * r.m13 + l.m12 * r.m23 + l.m13 * r.m33,
		l.m20 * r.m00 + l.m21 * r.m10 + l.m22 * r.m20 + l.m23 * r.m30,
		l.m20 * r.m01 + l.m21 * r.m11 + l.m22 * r.m21 + l.m23 * r.m31,
		l.m20 * r.m02 + l.m21 * r.m12 + l.m22 * r.m22 + l.m23 * r.m32,
		l.m20 * r.m03 + l.m21 * r.m13 + l.m22 * r.m23 + l.m23 * r.m33,
		l.m30 * r.m00 + l.m31 * r.m10 + l.m32 * r.m20 + l.m33 * r.m30,
		l.m30 * r.m01 + l.m31 * r.m11 + l.m32 * r.m21 + l.m33 * r.m31,
		l.m30 * r.m02 + l.m31 * r.m12 + l.m32 * r.m22 + l.m33 * r.m32,
		l.m30 * r.m03 + l.m31 * r.m13 + l.m32 * r.m23 + l.m33 * r.m33
	};
}

Matrix4 Mazeline::Mat4Identity() {
	return {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
}