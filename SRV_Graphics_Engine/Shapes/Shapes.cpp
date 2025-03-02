#include "Shapes.h"

#include <DirectXMath.h>
#include <math.h>

#define M_PI 3.14159265358979323846   // pi




std::tuple<std::vector<Vertex3D>, std::vector<DWORD>>
Shapes::GetCircleShape(float radius, int tesselation)
{
	std::vector<Vertex3D> vertices{};
	std::vector<DWORD> indices{};
	indices.resize(tesselation * 3);

	vertices.push_back({ Vector3D(0, 0, 0), ColorRGB(1,1,1) }); // center point
	float theta;

	for (int i = 1; i < tesselation + 1; ++i)
	{
		theta = (M_PI * 2 * (tesselation - i)) / static_cast<float>(tesselation);
		vertices.push_back({ Vector3D(radius * cos(theta), radius * sin(theta), 1.0f), ColorRGB(1.0f, 1.0f, 1.0f) });
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

std::tuple<std::vector<Vertex3D>, std::vector<DWORD>> Shapes::GetCubeShape(float size)
{
	std::vector<Vertex3D> cubeVertices
	{
		Vertex3D({ -size / 2, -size / 2, -size / 2}, GREEN),
		Vertex3D({ -size / 2,  size / 2, -size / 2}, GREEN),
		Vertex3D({size / 2,  size / 2, -size / 2}, GREEN),
		Vertex3D({size / 2, -size / 2, -size / 2}, GREEN),

		Vertex3D({size / 2, size / 2,  size / 2}, RED),
		Vertex3D({-size / 2,  size / 2,  size / 2}, RED),
		Vertex3D({ -size / 2,  -size / 2,  size / 2}, RED),
		Vertex3D({ size / 2, -size / 2,  size / 2}, RED),

		Vertex3D({-size / 2, -size / 2, -size / 2}, BLUE),
		Vertex3D({-size / 2,  -size / 2, size / 2}, BLUE),
		Vertex3D({-size / 2,  size / 2, size / 2}, BLUE),
		Vertex3D({-size / 2, size / 2,  -size / 2}, BLUE),

		Vertex3D({ size / 2, size / 2,  -size / 2}, YELLOW),
		Vertex3D({ size / 2,  -size / 2,  -size / 2}, YELLOW),
		Vertex3D({ size / 2,  -size / 2, size / 2}, YELLOW),
		Vertex3D({ size / 2, size / 2, size / 2}, YELLOW),

		Vertex3D({-size / 2,  size / 2, -size / 2}, PURPLE),
		Vertex3D({ size / 2,  size / 2, -size / 2}, PURPLE),
		Vertex3D({ size / 2,  size / 2,  size / 2}, PURPLE),
		Vertex3D({-size / 2,  size / 2,  size / 2}, PURPLE),

		Vertex3D({-size / 2, -size / 2, -size / 2}, ORANGE),
		Vertex3D({ size / 2, -size / 2, -size / 2}, ORANGE),
		Vertex3D({ size / 2, -size / 2,  size / 2}, ORANGE),
		Vertex3D({-size / 2, -size / 2,  size / 2}, ORANGE)
	};

	std::vector<DWORD> cubeIndices
	{
		0, 1, 2,
		0, 2, 3,

		4, 5, 6,
		4, 6, 7,

		8, 9, 10,
		8, 10, 11,

		14, 13, 12,
		15, 14, 12,

		19, 18, 17,
		19, 17, 16,

		20, 21, 22,
		20, 22, 23
	};

	return { cubeVertices, cubeIndices };
}

std::tuple<std::vector<Vertex3D>, std::vector<DWORD>> Shapes::GetSphereShape(float radius, int stackCount, int sliceCount, ColorRGB color)
{
	std::vector<Vertex3D> vertices{};
	std::vector<DWORD> indices{};

	for (int i = 0; i <= stackCount; ++i) 
	{
		float theta = static_cast<float>(i) / stackCount * DirectX::XM_PI;
		float sinTheta = sinf(theta);
		float cosTheta = cosf(theta);

		for (int j = 0; j <= sliceCount; ++j) 
		{
			float phi = static_cast<float>(j) / sliceCount * 2.0f * DirectX::XM_PI;
			float sinPhi = sinf(phi);
			float cosPhi = cosf(phi);

			Vertex3D vertex;
			vertex.position.x = radius * sinTheta * cosPhi; 
			vertex.position.y = radius * cosTheta;          
			vertex.position.z = radius * sinTheta * sinPhi; 
			vertex.color.x = color.r;
			vertex.color.y = color.g;
			vertex.color.z = color.b;

			vertices.push_back(vertex);
		}
	}

	for (int i = 0; i < stackCount; ++i) 
	{
		for (int j = 0; j < sliceCount; ++j) 
		{
			int first = (i * (sliceCount + 1)) + j;
			int second = first + sliceCount + 1;

			indices.push_back(first);
			indices.push_back(second);
			indices.push_back(first + 1);

			indices.push_back(second);
			indices.push_back(second + 1);
			indices.push_back(first + 1);
		}
	}

	return {vertices, indices};
}
