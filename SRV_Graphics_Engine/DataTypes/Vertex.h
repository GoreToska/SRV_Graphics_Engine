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

	TVertex(Vector3D position, float u, float v, Vector3D normal) :
		position(position.x, position.y, position.z),
		textCoord(u, v),
		normal(normal.x, normal.y, normal.z) {};

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 textCoord;
	DirectX::XMFLOAT3 normal;
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

