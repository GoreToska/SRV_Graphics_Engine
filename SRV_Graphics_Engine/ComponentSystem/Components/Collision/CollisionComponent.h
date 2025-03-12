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
	virtual BoundingVolume& GetBoundingVolume() = 0;
	void SetRadius(const float& radius);
	float GetRadius();

	GameObject* GetGameObject() const;

	bool Intersects(CollisionComponent& other) ;

	MulticastDelegate<CollisionComponent*> OnCollisionEnter;

private:
	TransformComponent* transform;
	GameObject* gameObject;
};