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

	// Shadowmap resolution
	static constexpr float ShadowmapSize = 2048;
	static constexpr int CascadesCount = 4;
};