#pragma once

#include<vector>
#include <d3d11.h>
#include <tuple>

#include"../DataTypes/Vertex.h"

class Shapes
{
public:
	static std::tuple<std::vector<Vertex3D>, std::vector<DWORD>>GetCircleShape(float radius, int tesselation);
	static std::tuple<std::vector<Vertex3D>, std::vector<DWORD>>GetCubeShape(const float& size, const ColorRGB& color);
	static std::tuple<std::vector<Vertex3D>, std::vector<DWORD>>GetSphereShape(float radius, int stackCount, int sliceCount, ColorRGB color);
};

