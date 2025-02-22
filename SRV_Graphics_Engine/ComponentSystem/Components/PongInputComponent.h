#pragma once

#include "IComponent.h"
#include <map>
#include "../../DataTypes/Vector3D.h"
#include "../GameObject.h"

class PongInputComponent : public IComponent
{
public:
	PongInputComponent(GameObject* posessedGameObject);

	void Update() override;
	ComponentType GetType() override;

	void SetInput(unsigned char upKey, unsigned char downKey);

private:
	void OnKeyDown(const unsigned char key);
	void OnKeyUp(const unsigned char key);

	TransformComponent* posessedObjectTransform;

	unsigned char up;
	unsigned char down;

	Vector3D movementVector = Vector3D();

	float movementSpeed = 0.005;
};

