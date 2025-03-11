#pragma once

#pragma region Includes
#include<DirectXCollision.h>
#include<DirectXMath.h>
#include <unordered_set>

#include "../IComponent.h"
#include "../../../DataTypes/Vector3D.h"
#include "../../../Delegates/Delegates.h"
#include "../../../DataTypes/Collision/BoundingVolume.h"

class GameObject;
class TransformComponent;
#pragma endregion


class CollisionComponent : public IComponent
{
public:
	CollisionComponent(GameObject* gameObject);

	virtual void Update(const float& deltaTime) override;
	virtual const BoundingVolume& GetBoundingVolume() const = 0;

	GameObject* GetGameObject() const;

	bool Intersects(const CollisionComponent& other) const;

	MulticastDelegate<CollisionComponent*> OnCollisionEnter;

private:
	TransformComponent* transform;
	GameObject* gameObject;
};