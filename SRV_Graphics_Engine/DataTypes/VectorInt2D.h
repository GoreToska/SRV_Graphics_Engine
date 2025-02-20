#pragma once


struct VectorInt2D
{
public:
	VectorInt2D(int x, int y) : x(x), y(y) {};

	int x;
	int y;
};

struct Vector2D
{
public:
	Vector2D() :x(0), y(0) {};
	Vector2D(float x, float y) : x(x), y(y) {};

	Vector2D operator + (const Vector2D& vector) const
	{
		return Vector2D{ x + vector.x, y + vector.y };
	}

	Vector2D operator += (const Vector2D& vector)
	{
		x += vector.x;
		y += vector.y;
		return *this;
	}

	float x;
	float y;
};

