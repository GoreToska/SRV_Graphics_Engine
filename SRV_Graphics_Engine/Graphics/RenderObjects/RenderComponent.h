#pragma once


#include <vector>
#include<wrl/client.h>
#include <d3d11.h>

#include "../../DataTypes/Vertex.h"

#pragma comment(lib, "d3d11.lib")


class RenderComponent
{
public:
	RenderComponent(std::vector<Vertex3D> vertices);
	void Render();

private:
	std::vector<Vertex3D> vertices{};

	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	// TODO: const buffer
};

