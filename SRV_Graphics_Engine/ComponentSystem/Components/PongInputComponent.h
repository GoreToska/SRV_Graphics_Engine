#pragma once

#include "IComponent.h"
#include "../../DataTypes/Vector3D.h"
#include "../GameObject.h"

#include <map>


class PongInputComponent : public IComponent
{
public:
	PongInputComponent(GameObject* posessedGameObject, float size);

	void Update(float deltaTime) override;

	void SetInput(unsigned char upKey, unsigned char downKey);
	void SetSpeed(float value);

private:
	void OnKeyDown(const unsigned char key);
	void OnKeyUp(const unsigned char key);

	TransformComponent* posessedObjectTransform;

	unsigned char up;
	unsigned char down;

	Vector3D movementVector = Vector3D();

	float movementSpeed = 0.005f;
	float size;
};

