#pragma once

#include <d3d11.h>
#include<DirectXMath.h>
#include <wrl/client.h>
#include "../../GameObject.h"

class ShadowMapCalculator
{
public:
	// Get 90 Degrees FOV
	static float GetFOV();

	// Get projection matrix for 90 Degrees FOV
	static DirectX::XMMATRIX GetProjectionMatrix();

	// Get perspective matrices for point light
	static DirectX::XMMATRIX GetViewMatrixDirectional(GameObject* gameObject);

	// Shadowmap resolution
	static constexpr float ShadowmapSize = 1024;
};