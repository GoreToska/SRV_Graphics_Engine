#pragma once

#include<vector>
#include <d3d11.h>
#include <tuple>

#include"../DataTypes/Vertex.h"

class Shapes2D
{
public:
	static std::tuple<std::vector<Vertex3D>, std::vector<DWORD>>  
		GetCircleShape(Vector3D center, float radius, int numSegments, ColorRGB color);
};

