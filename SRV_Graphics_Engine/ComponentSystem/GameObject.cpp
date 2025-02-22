#include "GameObject.h"
#include <iostream>

GameObject::GameObject()
{
	AddComponent(new TransformComponent());
}

void GameObject::Update()
{
	for (auto& component : components)
	{
		component.second->Update();
	}
}

void GameObject::AddComponent(IComponent* component)
{
	std::cout << component->GetType() << std::endl;
	components[component->GetType()] = component;

	TransformComponent* transform = dynamic_cast<TransformComponent*>(component);
	if (transform != nullptr)
	{
		this->transform = transform;
		return;
	}
}

IComponent* GameObject::GetComponent(IComponent::ComponentType type)
{
	auto foundComponent = components.find(type);

	if (foundComponent != components.end())
	{
		return foundComponent->second;
	}
	else
	{
		//Logger::LogError("Failed to get component of type " + std::to_string(type));
		return nullptr;
	}
}

TransformComponent* GameObject::GetTransform()
{
	return transform;
}