#pragma once

#pragma region Includes

#include <map>
#include <typeindex>
#include <vector>

#include "./Components/TransformComponent.h"
#include "Components/IComponent.h"


#pragma endregion


class GameObject
{
public:
	GameObject()
	{
		transform = AddComponent<TransformComponent>(new TransformComponent(this));
	}

	GameObject(Vector3D position)
	{
		transform = AddComponent<TransformComponent>(new TransformComponent(this));
		transform->SetPosition(position);
	}

	void Update(float deltaTime)
	{
		for (auto it = components.begin(); it != components.end(); ++it)
		{
			it->second->Update(deltaTime);
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
		for (auto it = components.begin(); it != components.end(); ++it)
		{
			T* result = dynamic_cast<T*>(it->second);

			if (result)
			{
				return result;
			}
		}

		return nullptr;
	}

	TransformComponent* GetTransform()
	{
		return transform;
	}

	std::vector<GameObject*> GetChildren()
	{
		return childGameObjects;
	}

	void AttachChild(GameObject* childObject)
	{
		childGameObjects.push_back(childObject);
		childObject->parent = this;
	}

	void DetachChild(GameObject* childObject)
	{
		auto it = std::find(childGameObjects.begin(), childGameObjects.end(), childObject);
		if (it != childGameObjects.end())
		{
			childGameObjects.erase(it);
			childObject->GetTransform()->SetPosition(childObject->GetTransform()->GetPosition() + transform->GetPosition());
			childObject->parent = nullptr;
		}
	}

	bool HasChild(GameObject* childObject)
	{
		auto it = std::find(childGameObjects.begin(), childGameObjects.end(), childObject);

		if (it != childGameObjects.end())
			return true;

		return false;
	}

	bool IsChild(GameObject* object)
	{
		auto it = std::find(object->GetChildren().begin(), object->GetChildren().end(), this);

		if (it != object->GetChildren().end())
			return true;

		return false;
	}

	GameObject* GetParent()
	{
		return parent;
	}

	GameObject* GetRoot()
	{
		if (parent)
			return parent->GetRoot();

		return this;
	}

private:
	GameObject* parent;
	std::map<std::type_index, IComponent*> components{};
	std::vector<GameObject*> childGameObjects{};
	TransformComponent* transform;
};



