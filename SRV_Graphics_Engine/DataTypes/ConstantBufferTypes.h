#pragma once

#include<d3d11.h>
#include<DirectXMath.h>

struct CB_VS_VertexShader
{
	CB_VS_VertexShader() {};
	CB_VS_VertexShader(DirectX::XMMATRIX mat) : matrix(mat) {}

	DirectX::XMMATRIX matrix;
};