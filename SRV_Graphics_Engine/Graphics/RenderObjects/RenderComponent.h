#pragma once


#include <vector>
#include<wrl/client.h>
#include <d3d11.h>

#include "../../DataTypes/Vertex.h"

#pragma comment(lib, "d3d11.lib")


class RenderComponent
{
public:
	RenderComponent(std::vector<Vertex3D> vertices, ID3D11Device* device, ID3D11DeviceContext* deivceContext);
	void Render();

private:
	std::vector<Vertex3D> vertices{};

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;

	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;


};

