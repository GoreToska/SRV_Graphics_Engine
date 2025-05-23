#pragma once

#include "../IComponent.h"
#include "../../../Graphics/ShaderManager/ShaderManager.h"
#include "../../../DataTypes/ConstantBufferTypes.h"
#include "../../GameObject.h"
#include "../../../Graphics/Buffers/ConstantBuffer.h"

class IRenderComponent : public IComponent
{
public:
	IRenderComponent(GameObject* gameObject, ShaderManager::ShaderType type);

	virtual void Update(const float& deltaTime) = 0;
	virtual void Render(bool setShaders = true);
	virtual void RenderNoShader();
	virtual void RenderForShadows();

	virtual int GetVertexCount() const = 0;
	virtual void SetVertexBufferContext() = 0;

	GameObject* GetGameObject();

protected:
	void UpdateLightBuffer();
	void UpdateCascadeShadowBuffer();
	void UpdateTransformBuffer(DirectX::XMMATRIX WorldMatrix, DirectX::XMMATRIX ViewMatrix, DirectX::XMMATRIX ProjectionMatrix);

	GameObject* gameObject = nullptr;

	ShaderManager::ShaderType shaderType;
	ConstantBuffer<VS_ObjectMatrixBuffer> objectMatrixBuffer = {};
	ConstantBuffer<VS_LightMatrixBuffer> lightMatrixBuffer = {};
	ConstantBuffer<PS_LightParamsBuffer> lightConstBuffer = {};
	ConstantBuffer<PS_CascadeShadowsBuffer> cascadeShadowsBuffer = {};
};