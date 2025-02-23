#pragma once

#include <map>
#include <typeindex>

#include "./Components/RenderComponent.h"
#include "./Components/TransformComponent.h"
#include "Components/IComponent.h"


class RenderComponent;
class TransformComponent;

class GameObject
{
public:
	GameObject()
	{
		transform = AddComponent<TransformComponent>(new TransformComponent());
	}

	void Update()
	{
		for (auto& component : components)
		{
			component.second->Update();
		}
	}

	template<typename T, typename = std::enable_if_t<std::is_base_of_v<IComponent, T>>>
	T* AddComponent(T* component)
	{
		components[std::type_index(typeid(T))] = component;
		return component;
	}

	template <typename T, typename = std::enable_if_t<std::is_base_of_v<IComponent, T>>>
	T* GetComponent()
	{
		auto it = components.find(std::type_index(typeid(T)));

		if (it != components.end())
		{
			return dynamic_cast<T*>(it->second);
		}
		else
		{
			//Logger::LogError("Failed to get component of type " + std::to_string(type));
			return nullptr;
		}
	}

	TransformComponent* GetTransform()
	{
		return transform;
	}

private:
	std::map<std::type_index, IComponent*> components;

	TransformComponent* transform;
};



