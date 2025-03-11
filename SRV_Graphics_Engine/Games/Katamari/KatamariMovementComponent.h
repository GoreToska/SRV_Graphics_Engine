#pragma once

#include "../../ComponentSystem/Components/IComponent.h"
#include "../../ComponentSystem/GameObject.h"


class KatamariMovementComponent : public IComponent
{
public:
	KatamariMovementComponent(GameObject* gameObject);
	void Update(const float& deltaTime);

private:
	void OnKeyPressed(const unsigned char key);
	void OnKeyReleased(const unsigned char key);
	void UpdateRotation(const float& deltaTime);
	void UpdatePosition(const float& deltaTime);

	Vector3D movementDirection;
	GameObject* gameObject;

	float speed = 0.01;

	const unsigned char upKey = 'W';
	const unsigned char downKey = 'S';
	const unsigned char leftKey = 'A';
	const unsigned char rightKey = 'D';
};

