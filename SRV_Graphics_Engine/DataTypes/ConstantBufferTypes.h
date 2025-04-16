#pragma once

#include<d3d11.h>
#include<DirectXMath.h>

struct MatrixBuffer
{
	MatrixBuffer() {};

	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
};

struct CB_PS_Light
{
	CB_PS_Light() {};

	DirectX::XMFLOAT3 ambientLightColor = { 1.0f,1.0f,1.0f };
	float ambientLightStrength = 0.1;
	DirectX::XMFLOAT3 dynamicLightColor /*= { 1.0f,0.0f,0.0f }*/;
	float dynamicLightStrength = 1;
	DirectX::XMFLOAT3 dynamicLightDirection = { -0.577f, 0.577f, -0.577f };
	DirectX::XMMATRIX lightWorld;
	DirectX::XMMATRIX lightView;
	DirectX::XMMATRIX lightProjection;
};