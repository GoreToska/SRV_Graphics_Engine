#pragma once

#include "../../ComponentSystem/Components/IComponent.h"
#include "../../ComponentSystem/GameObject.h"


class OrbitalMovementComponent : public IComponent
{
public:
	OrbitalMovementComponent(GameObject* owner, GameObject* orbit, Vector3D axis, float speed, float radius);
	void Update(const float& deltaTime) override;

private:
	GameObject* orbit;
	GameObject* owner;
	Vector3D axis;
	float speed;
	float radius;
	float theta;
};

