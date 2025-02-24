#pragma once

#include<vector>
#include <d3d11.h>

#include"../DataTypes/Vertex.h"

class Shapes2D
{
public:
	static std::vector<Vertex3D> GetCircleShape(float radius, int tesselation);
};

