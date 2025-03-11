#pragma once

#include "../IComponent.h"
#include "../../GameObject.h"
#include "../../../Graphics/Camera.h"

class TopDownCameraComponent : public IComponent
{
public:
	TopDownCameraComponent(Camera* camera, GameObject* followObject, Vector3D offset);

	void Update(const float& deltaTime) override;

private:
	void UpdatePosition();

	GameObject* followObject;
	Camera* camera;
	Vector3D offset;
};

