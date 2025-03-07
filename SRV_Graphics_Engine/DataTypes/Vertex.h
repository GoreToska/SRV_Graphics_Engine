#pragma once

#include<DirectXMath.h>

#include "VectorInt2D.h"
#include "ColorRGB.h"
#include "Vector3D.h"

struct Vertex2D
{
	Vertex2D() {};
	Vertex2D(Vector2D position, ColorRGB color) :
		position(position.x, position.y),
		color(color.r, color.g, color.b) {};

	DirectX::XMFLOAT2 position;
	DirectX::XMFLOAT3 color;
};

struct Vertex3D
{
	Vertex3D() {};
	Vertex3D(Vector3D position, ColorRGB color) :
		position(position.x, position.y, position.z),
		color(color.r, color.g, color.b) {};

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 color;
};


