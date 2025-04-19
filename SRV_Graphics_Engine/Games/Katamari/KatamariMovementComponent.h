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
	void UpdateHeight(const float& deltaTime);
	void UpdatePosition(const float& deltaTime);
	float GetLandscapeHeight(float x, float z);
	Vector3D CalculateMovementDirection();

	Vector3D movementDirection;
	GameObject* gameObject;

	float speed = 0.002;

	const unsigned char upKey = 'W';
	const unsigned char downKey = 'S';
	const unsigned char leftKey = 'A';
	const unsigned char rightKey = 'D';
};

