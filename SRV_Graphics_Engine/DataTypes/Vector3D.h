#pragma once

#include <DirectXMath.h>
#include <cmath>

struct Vector3D
{
public:
	Vector3D() : x(0), y(0), z(0) {};
	Vector3D(float x, float y, float z) : x(x), y(y), z(z) {};
	Vector3D(DirectX::XMVECTOR& vector) : x(vector.m128_f32[0]), y(vector.m128_f32[1]), z(vector.m128_f32[2]) {};
	static Vector3D ZeroVector();

	Vector3D operator + (const Vector3D& vector) const
	{
		return Vector3D{ x + vector.x, y + vector.y, z + vector.z };
	}

	Vector3D operator += (const Vector3D& vector)
	{
		x += vector.x;
		y += vector.y;
		z += vector.z;
		return *this;
	}

	Vector3D operator - (const Vector3D& vector) const
	{
		return Vector3D{ x - vector.x, y - vector.y, z - vector.z };
	}

	bool operator == (const Vector3D& vector) const
	{
		return  x == vector.x && y == vector.y && z == vector.z;
	}

	Vector3D operator -= (const Vector3D& vector)
	{
		x -= vector.x;
		y -= vector.y;
		z -= vector.z;
		return *this;
	}

	Vector3D operator * (const float& value) const
	{
		return Vector3D{ x * value, y * value, z * value };
	}

	Vector3D operator * (const Vector3D& value) const
	{
		return Vector3D{ x * value.x, y * value.y, z * value.z };
	}

	Vector3D Normalized() const
	{
		float length = std::sqrtf(x * x + y * y + z * z);

		if (length == 0.0f)
			return Vector3D::ZeroVector();

		return Vector3D(x / length, y / length, z / length);
	}

	float LengthSquared() const
	{
		return x * x + y * y + z * z;
	}

	float GetYawAngle() const
	{
		return std::atan2(x, z);
	}

	float x;
	float y;
	float z;
};