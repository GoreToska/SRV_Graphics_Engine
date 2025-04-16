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
	virtual void Render();
	virtual void RenderForShadows();

	void UpdateLightBuffer();
	void UpdateTransformBuffer(DirectX::XMMATRIX WorldMatrix, DirectX::XMMATRIX ViewMatrix, DirectX::XMMATRIX ProjectionMatrix);
	virtual int GetVertexCount() const = 0;
	virtual void SetVertexBufferContext() = 0;

	GameObject* GetGameObject();

protected:
	GameObject* gameObject = nullptr;

	ShaderManager::ShaderType shaderType;
	ConstantBuffer<MatrixBuffer> constBuffer = {};
	ConstantBuffer<CB_PS_Light> lightConstBuffer = {};
};