#pragma once

#include <map>

#include "./Components/RenderComponent.h"
#include "./Components/TransformComponent.h"
#include "Components/IComponent.h"


class RenderComponent;
class TransformComponent;

class GameObject
{
public:
	GameObject();

	void Update();
	void AddComponent(IComponent* component);
	IComponent* GetComponent(IComponent::ComponentType type);

	TransformComponent* GetTransform();

private:
	std::map<IComponent::ComponentType, IComponent*> components;

	TransformComponent* transform;
};



