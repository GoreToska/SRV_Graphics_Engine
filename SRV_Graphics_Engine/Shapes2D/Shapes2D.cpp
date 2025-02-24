#include "Shapes2D.h"

#include <DirectXMath.h>
#include <math.h>

#define M_PI 3.14159265358979323846   // pi




std::tuple<std::vector<Vertex3D>, std::vector<DWORD>>
Shapes2D::GetCircleShape(float radius, int tesselation)
{
	std::vector<Vertex3D> vertices{};
	std::vector<DWORD> indices{};
	indices.resize(tesselation * 3);

	vertices.push_back({ Vector3D(0, 0, 1), ColorRGB(1,1,1) }); // center point
	float theta;

	for (int i = 1; i < tesselation + 1; ++i)
	{
		theta = (M_PI * 2 * (tesselation - i)) / static_cast<float>(tesselation);
		vertices.push_back({ Vector3D(radius / 1.33 * cos(theta), radius * sin(theta), 1.0f), ColorRGB(1.0f, 1.0f, 1.0f) });
	}

	int indiceHelperNum1 = 1;
	int indiceHelperNum2 = 2;

	for (int i = 0; i < tesselation * 3 - 3; i += 3)
	{
		indices[i] = 0;
		indices[i + 1] = indiceHelperNum1;
		indices[i + 2] = indiceHelperNum2;

		indiceHelperNum1++;
		indiceHelperNum2++;
	}

	indices[tesselation * 3 - 3] = 0;
	indices[tesselation * 3 - 2] = tesselation;
	indices[tesselation * 3 - 1] = 1;

	return { vertices, indices };
}
