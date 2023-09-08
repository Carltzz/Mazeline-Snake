#ifndef M_MATRIX_H
#define M_MATRIX_H
#include "MathConstants.h"

namespace Mazeline {
	struct Matrix2;
	struct Matrix3;
	struct Matrix4;
	struct Vector3;
	struct Quaternion;

	Matrix4 CreatePerspectiveMatrix(float fov, float aspect, float zNear, float zFar);
	Matrix4 CreateOrthographicMatrix(int width, int height, float zNear, float zFar);
	Matrix4 CreateTranslationMatrix(Vector3 translation);
	Matrix4 CreateScaleMatrix(Vector3 scale);
	Matrix4 CreateFromQuaternion(Quaternion q);
	Matrix4 CreateEulerRotationXYZ(Vector3 euler);
	Matrix4 CreateEulerRotationXZY(Vector3 euler);
	Matrix4 CreateEulerRotationYXZ(Vector3 euler);
	Matrix4 CreateEulerRotationYZX(Vector3 euler);
	Matrix4 CreateEulerRotationZYX(Vector3 euler);
	Matrix4 CreateEulerRotationZXY(Vector3 euler);
	Matrix4 CreateEulerRotationXY(float X, float Y);
	Matrix4 CreateEulerRotationYX(float X, float Y);
	Matrix4 CreateEulerRotationXZ(float X, float Z);
	Matrix4 CreateEulerRotationZX(float X, float Z);
	Matrix4 CreateEulerRotationYZ(float Y, float Z);
	Matrix4 CreateEulerRotationZY(float Y, float Z);
	Matrix4 CreateEulerRotationX(float X);
	Matrix4 CreateEulerRotationY(float Y);
	Matrix4 CreateEulerRotationZ(float Z);
	Matrix4 CreateViewMatrix(Vector3 pos, Vector3 rot, Vector3 scale);
	Matrix4 MultiplyMat4(Matrix4& left, Matrix4& right);
	Matrix4 Mat4Identity();

	struct Matrix2 {
		float m00, m01;
		float m10, m11;
	};

	struct Matrix3 {
		float m00, m01, m02;
		float m10, m11, m12;
		float m20, m21, m22;
	};

	struct Matrix4 {
		float m00, m01, m02, m03;
		float m10, m11, m12, m13;
		float m20, m21, m22, m23;
		float m30, m31, m32, m33;

		Matrix4 Transpose() {
			return {
				m00, m10, m20, m30,
				m01, m11, m21, m31,
				m02, m12, m22, m32,
				m03, m13, m23, m33
			};
		}

		Matrix4 operator*(Matrix4 right) {
			return MultiplyMat4(*this, right);
		}
	};
}

#endif