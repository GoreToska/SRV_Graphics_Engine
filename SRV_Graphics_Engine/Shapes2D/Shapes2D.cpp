#include "Shapes2D.h"

#include <DirectXMath.h>


#ifndef PI
#define PI 3.14159265359
#endif // !PI


std::tuple<std::vector<Vertex3D>, std::vector<DWORD>>
Shapes2D::GetCircleShape(Vector3D center, float radius, int numSegments, ColorRGB color)
{
	std::vector<Vertex3D> vertices{};
	std::vector<DWORD> indices{};

	vertices.push_back({ center, color });

	for (int i = 0; i <= numSegments; ++i)
	{
		float theta = 2.0f * PI * float(i) / float(numSegments);
		float x = radius * cosf(theta);
		float y = radius * sinf(theta);
		vertices.push_back({ { center.x + x, center.y + y, center.z }, color });

		if (i > 0)
		{
			indices.push_back(0); // Центр
			indices.push_back(i); // Текущая вершина
			indices.push_back(i + 1); // Следующая вершина
		}
	}

	return std::make_tuple(vertices, indices);
}
