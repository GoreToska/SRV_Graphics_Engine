#pragma once

#include<d3d11.h>
#include<DirectXMath.h>

struct CB_VS_VertexShader
{
	CB_VS_VertexShader() {};
	CB_VS_VertexShader(DirectX::XMMATRIX mat) : matrix(mat) {}

	DirectX::XMMATRIX matrix;
};

struct CB_PS_Light
{
	CB_PS_Light() {};
	CB_PS_Light(DirectX::XMFLOAT3 ambientColor, float ambientStrength)
		: ambientLightColor(ambientColor), ambientLightStrength(ambientStrength) {};

	DirectX::XMFLOAT3 ambientLightColor;
	float ambientLightStrength;
};