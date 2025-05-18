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

	static Matrix GetDirectionalLightViewProj(Vector3D direction, float nearZ, float farZ);

	// Shadowmap resolution
	static constexpr float ShadowmapSize = 4096;
	static constexpr size_t CascadeCount = 4;

	static constexpr float shadowCascadeDistanceMultipliers[CascadeCount] = {0.05, 0.1, 0.3, 1};
};