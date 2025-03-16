#include "IRenderComponent.h"
#include "../../../Graphics/Device/GraphicsDevice.h"

IRenderComponent::IRenderComponent(ShaderManager::ShaderType type)
	: shaderType(type)
{
}

void IRenderComponent::Render()
{
	DeviceContext->IASetInputLayout(ShaderManager::GetInstance().GetVS(shaderType)->GetInputLayout());
	DeviceContext->VSSetShader(ShaderManager::GetInstance().GetVS(shaderType)->GetShader(), NULL, 0);
	DeviceContext->PSSetShader(ShaderManager::GetInstance().GetPS(shaderType)->GetShader(), NULL, 0);
}
