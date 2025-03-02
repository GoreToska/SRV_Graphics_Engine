#pragma once

#include "../../DataTypes/Vector3D.h"
#include "../../ComponentSystem/Components/IComponent.h"
#include "../../ComponentSystem/GameObject.h"

#include <map>


class PongInputComponent : public IComponent
{
public:
	PongInputComponent(GameObject* posessedGameObject);

	void Update(const float& deltaTime) override;

	void SetInput(unsigned char upKey, unsigned char downKey);

private:
	void OnKeyDown(const unsigned char key);
	void OnKeyUp(const unsigned char key);

	TransformComponent* posessedObjectTransform;

	unsigned char up;
	unsigned char down;

	Vector3D movementVector = Vector3D();

	float movementSpeed = 0.005f;
};

