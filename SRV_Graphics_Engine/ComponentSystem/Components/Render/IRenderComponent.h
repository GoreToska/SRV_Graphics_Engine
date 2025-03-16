#pragma once

#include "../IComponent.h"
#include "../../../Graphics/ShaderManager/ShaderManager.h"

class IRenderComponent : public IComponent
{
public:
	IRenderComponent(ShaderManager::ShaderType type);

	virtual void Update(const float& deltaTime) = 0;
	virtual void Render();

protected:
	ShaderManager::ShaderType shaderType;
};