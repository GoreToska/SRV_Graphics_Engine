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
	float GetLightAttenuationConst() const;
	float GetLightAttenuationLinear() const;
	float GetLightAttenuationExponent() const;

private:
	DirectX::XMFLOAT3 lightColor = DirectX::XMFLOAT3(1, 0, 0);
	float lightStrength = 1.0f;

	float attenuation_const = 1.0f;
	float attenuation_linear = 0.1f;
	float attenuation_exponent = 0.1f;

};