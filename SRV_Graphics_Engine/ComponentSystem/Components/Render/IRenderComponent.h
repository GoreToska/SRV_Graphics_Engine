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
	void UpdateLightBuffer();
	void UpdateTransformBuffer();
	virtual int GetVertexCount() const = 0;

	GameObject* GetGameObject();

protected:
	GameObject* gameObject = nullptr;

	ShaderManager::ShaderType shaderType;
	ConstantBuffer<MatrixBuffer> constBuffer = {};
	ConstantBuffer<CB_PS_Light> lightConstBuffer = {};
};