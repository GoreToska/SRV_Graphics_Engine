#pragma once

#include "../Render/MeshRendererComponent.h"
#include "../../GameObject.h"

class PointLightComponent : public MeshRendererComponent
{
public:
	PointLightComponent(GameObject* gameObject);

	void Update(const float& deltaTime) override;
	void Render() override;

	void SetLightColor(DirectX::XMFLOAT3& color);
	void SetLightColor(DirectX::XMFLOAT3 color);
	void SetLightStrength(float strength);
	DirectX::XMFLOAT3& GetLightColor();
	float GetLightStrength();

private:
	DirectX::XMFLOAT3 lightColor = DirectX::XMFLOAT3(1, 0, 0);
	float lightStrength = 1;
};