#pragma once

struct Vector3D
{
public:
	Vector3D() : x(0), y(0), z(0) {};
	Vector3D(float x, float y, float z) : x(x), y(y), z(z) {};

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

	float x;
	float y;
	float z;
};