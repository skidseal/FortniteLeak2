#pragma once
#include <cmath>
#include <Windows.h>
#include <d3d9types.h>
#include <iostream>

#define PI (3.14159265358979323846264338327950288419716939937510)

class Vector3 {
public:
	Vector3();
	Vector3(float, float, float);
	~Vector3();

	float x{ }, y{ }, z{ };

	void make_absolute() {
		x = std::abs(x);
		y = std::abs(y);
		z = std::abs(z);
	}

	Vector3& operator+=(const Vector3& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	Vector3& operator-=(const Vector3& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	Vector3& operator*=(const float v) {
		x *= v;
		y *= v;
		z *= v;
		return *this;
	}

	Vector3 operator+(const Vector3& v) {
		return Vector3{ x + v.x, y + v.y, z + v.z };
	}

	Vector3 operator-(const Vector3& v) {
		return Vector3{ x - v.x, y - v.y, z - v.z };
	}

	Vector3& operator+(const float& v) {
		x = x + v;
		y = y + v;
		z = z + v;
		return *this;
	}

	Vector3& operator-(const float& v) {
		x = x - v;
		y = y - v;
		z = z - v;
		return *this;
	}

	Vector3 operator*(const float v) {
		return Vector3{ x * v, y * v, z * v };
	}

	Vector3 operator/(const float fl) const {
		return Vector3(x / fl, y / fl, z / fl);
	}

	Vector3 operator+(const Vector3& v) const {
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	bool operator==(const Vector3& v) const {
		return x == v.x && y == v.y && z == v.z;
	}

	bool operator!=(const Vector3& v) const {
		return x != v.x || y != v.y || z != v.z;
	}

	Vector3 operator*(const float fl) const {
		return Vector3(x * fl, y * fl, z * fl);
	}

	Vector3 operator*(const Vector3& v) const {
		return Vector3(x * v.x, y * v.y, z * v.z);
	}

	Vector3 operator-(const Vector3& v) const {
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	Vector3& operator/=(const float fl) {
		x /= fl;
		y /= fl;
		z /= fl;
		return *this;
	}

	float operator[](const int i) const {
		// ReSharper disable CppCStyleCast
		return ((float*)this)[i];
		// ReSharper restore CppCStyleCast
	}

	float& operator[](const int i) {
		return reinterpret_cast<float*>(this)[i];
	}


	inline float Customsqrtf_(float x)
	{
		union { float f; uint32_t i; } z = { x };
		z.i = 0x5f3759df - (z.i >> 1);
		z.f *= (1.5f - (x * 0.5f * z.f * z.f));
		z.i = 0x7EEEEEEE - z.i;
		return z.f;
	}


	double Custompowf_(double x, int y)
	{
		double temp;
		if (y == 0)
			return 1;
		temp = Custompowf_(x, y / 2);
		if ((y % 2) == 0) {
			return temp * temp;
		}
		else {
			if (y > 0)
				return x * temp * temp;
			else
				return (temp * temp) / x;
		}
	}

	__forceinline float DistanceFrom(const Vector3& Other) const {
		const Vector3& a = *this;
		float dx = (a.x - Other.x);
		float dy = (a.y - Other.y);
		float dz = (a.z - Other.z);

		return (sqrt((dx * dx) + (dy * dy) + (dz * dz)));
	}


	inline float Distance(Vector3 v) {
		return float(Customsqrtf_(Custompowf_(v.x - x, 2.0) + Custompowf_(v.y - y, 2.0) + Custompowf_(v.z - z, 2.0)));
	}

	inline float distance() {
		return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
	}
	/*
	void clamp() {
		if (x > 75.f) x = 75.f;
		else if (x < -75.f) x = -75.f;
		if (z < -180) z += 360.0f;
		else if (z > 180) z -= 360.0f;

		y = 0.f;
	}
	*/
	inline double Length() {
		return (double)(Customsqrtf_(x * x + y * y + z * z));
	}

	/*
	void normalize() {
		while (this->x > 89.0f)
			this->x -= 180.0f;

		while (this->x < -89.0f)
			this->x += 180.0f;

		while (this->y > 180.0f)
			this->y -= 360.0f;

		while (this->y < -180.0f)
			this->y += 360.0f;
	}
	*/
	/*
	[[nodiscard]] float dot(const Vector3& other) const {
		return x * other.x + y * other.y + z * other.z;
	}
	*/
	[[nodiscard]] bool empty() const {
		return x == 0 || y == 0 || z == 0;
	}
	/*
	void init(const float ix = 0.0f, const float iy = 0.0f, const float iz = 0.0f) {
		x = ix;
		y = iy;
		z = iz;
	}
	*/

	Vector3 ClampAngles() {

		if (this->y < -180.0f)
			this->y += 360.0f;

		if (this->y > 180.0f)
			this->y -= 360.0f;

		if (this->x < -74.0f)
			this->x = -74.0f;

		if (this->x > 74.0f)
			this->x = 74.0f;

		return *this;
	}

	static Vector3 CalcAngle(Vector3 cameraLocation, Vector3 boneLocation) {

		Vector3 result;

		auto delta = boneLocation - cameraLocation;
		auto distance = delta.distance();

		result.y = atan2f(delta.y, delta.x) * (180.0f / PI);
		result.x = (-((acosf((delta.z / distance)) * 180.0f / PI) - 90.0f));
		result.z = NULL;

		return result;
	}
};

struct FMatrix
{
	float M[4][4];
};
static FMatrix* myMatrix = new FMatrix();