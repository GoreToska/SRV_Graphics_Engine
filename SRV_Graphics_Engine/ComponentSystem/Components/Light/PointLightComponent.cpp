#include "PointLightComponent.h"
#include "../../../DataTypes/ModelData.h"
#include <iostream>

PointLightComponent::PointLightComponent(GameObject* gameObject)
	: MeshRendererComponent(ModelData("Data\\Models\\Light\\PointLight\\PointLight.obj",
		L"Data\\Models\\Light\\PointLight\\PointLight.png"), gameObject, ShaderManager::ShaderType::Texture)
{
	lightConstBuffer.GetData()->dynamicLightColor = lightColor;
	lightConstBuffer.GetData()->dynamicLightPosition =
	{ gameObject->GetTransform()->GetPosition().x,
	gameObject->GetTransform()->GetPosition().y,
	gameObject->GetTransform()->GetPosition().z };
	lightConstBuffer.GetData()->dynamicLightStrength = lightStrength;
}

void PointLightComponent::Update(const float& deltaTime)
{
	if (!IsEnabled())
		return;

	MeshRendererComponent::Update(deltaTime);

	lightConstBuffer.GetData()->dynamicLightColor = lightColor;
	lightConstBuffer.GetData()->dynamicLightPosition =
	{ gameObject->GetTransform()->GetPosition().x,
	gameObject->GetTransform()->GetPosition().y,
	gameObject->GetTransform()->GetPosition().z };
	lightConstBuffer.GetData()->dynamicLightStrength = lightStrength;
}

void PointLightComponent::Render()
{
	if (!IsEnabled())
		return;

	MeshRendererComponent::Render();
}

void PointLightComponent::SetLightColor(DirectX::XMFLOAT3& color)
{
	lightColor = color;
}

void PointLightComponent::SetLightColor(DirectX::XMFLOAT3 color)
{
	lightColor = color;
}

void PointLightComponent::SetLightStrength(float strength)
{
	lightStrength = strength;
}

DirectX::XMFLOAT3& PointLightComponent::GetLightColor()
{
	return lightColor;
}

float PointLightComponent::GetLightStrength()
{
	return lightStrength;
}

float PointLightComponent::GetLightAttenuationConst() const
{
	return attenuation_const;
}

float PointLightComponent::GetLightAttenuationLinear() const
{
	return attenuation_linear;
}

float PointLightComponent::GetLightAttenuationExponent() const
{
	return attenuation_exponent;
}
