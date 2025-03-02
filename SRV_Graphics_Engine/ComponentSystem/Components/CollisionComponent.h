#pragma once

#pragma region Includes
#include<DirectXCollision.h>
#include<DirectXMath.h>

#include "IComponent.h"
#include "../../DataTypes/Vector3D.h"
#include "../../Delegates/Delegates.h"
#include <unordered_set>

class GameObject;
class TransformComponent;
#pragma endregion


class CollisionComponent : public IComponent
{
public:
	enum CollisionChannel
	{
		Channel_1,
		Channel_2,
		Channel_3,
	};

	CollisionComponent(GameObject* gameObject, Vector3D firstPoint, Vector3D secondPoint, CollisionChannel collisionChannel = CollisionChannel::Channel_1);

	void Update(const float& deltaTime) override;

	const DirectX::BoundingBox& GetBoundingVolume();
	GameObject* GetGameObject() const;

	MulticastDelegate<CollisionComponent*, CollisionComponent*> OnCollisionEnter;

	bool IsEnabled = true;

	CollisionChannel collisionChannel;

	void AddIgnoreChannel(CollisionChannel channel);

private:
	std::unordered_set<CollisionChannel> ignoreChannels = {};

	DirectX::BoundingBox boxCollider;
	TransformComponent* transform;
	GameObject* gameObject;
};