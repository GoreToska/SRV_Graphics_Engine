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
	void UpdateTransformBuffer(Matrix WorldMatrix, Matrix ViewMatrix, Matrix ProjectionMatrix);

	GameObject* gameObject = nullptr;

	ShaderManager::ShaderType shaderType;
	ConstantBufferW<VS_ObjectMatrixBuffer> objectMatrixBuffer = {};
	ConstantBufferW<VS_LightMatrixBuffer> lightMatrixBuffer = {};
	ConstantBufferW<PS_LightParamsBuffer> lightConstBuffer = {};
	ConstantBufferW<PS_CascadeShadowsBuffer> cascadeShadowsBuffer = {};
};