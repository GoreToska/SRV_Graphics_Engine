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
		color(color.r, color.g, color.b, 1) {};

	DirectX::XMFLOAT2 position;
	DirectX::XMFLOAT4 color;
};

struct TVertex
{
	TVertex() {};

	TVertex(Vector3D position, float u, float v) :
		position(position.x, position.y, position.z),
		textCoord(u, v) {};

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 textCoord;
};

struct CVertex
{
	CVertex() {};

	CVertex(Vector3D position, ColorRGB color) :
		position(position.x, position.y, position.z),
		color(color.r, color.g, color.b, 1) {};

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 color;
};

