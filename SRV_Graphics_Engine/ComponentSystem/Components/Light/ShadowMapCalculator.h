#pragma once

#include <d3d11.h>
#include<DirectXMath.h>
#include <wrl/client.h>
#include "../../GameObject.h"

class ShadowMapCalculator
{
public:
	static float GetFOV();

	static DirectX::XMMATRIX GetProjectionMatrix();

	static DirectX::XMMATRIX GetViewMatrixDirectional(GameObject* gameObject);

	static void GetDirectionalLightMatrices(GameObject* gameObject, std::vector<Matrix>& projections, Matrix& view);

	// Shadowmap resolution
	static constexpr float ShadowmapSize = 1024;
	static constexpr size_t CascadeCount = 4;
};