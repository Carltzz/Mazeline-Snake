#ifndef F_VECTOR_H
#define F_VECTOR_H

namespace Mazeline {
	struct Vector2 {
		float X, Y;

		Vector2() {
			X = 0;
			Y = 0;
		}
		
		Vector2(float x, float y) {
			X = x; 
			Y = y;
		}

		Vector2& operator+=(const Vector2& right) {
			X = X + right.X;
			Y = Y + right.Y;
			return *this;
		}

		Vector2 operator-() {
			return {
				-X,
				-Y
			};
		}
	};

	struct Vector3 {
		float X, Y, Z;

		Vector3() {
			X = 0;
			Y = 0;
			Z = 0;
		}

		Vector3(float x, float y, float z) {
			X = x;
			Y = y;
			Z = z;
		}

		Vector3 operator-() {
			return {
				-X,
				-Y,
				-Z,
			};
		}
	};

	struct Vector4 {
		float X, Y, Z, W;

		Vector4 operator-() {
			return {
				-X,
				-Y,
				-Z,
				-W,
			};
		}
	};
}
#endif