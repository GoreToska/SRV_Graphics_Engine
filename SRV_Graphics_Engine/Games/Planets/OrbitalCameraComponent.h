#pragma once

#include "../../ComponentSystem/Components/IComponent.h"
#include "../../Graphics/Camera.h"
#include "../../DataTypes/Vector3D.h"
#include "../../ComponentSystem/GameObject.h"

class OrbitalCameraComponent : public IComponent
{
public:
	OrbitalCameraComponent(Camera* camera, GameObject* lookAtObject, float sensitivity);

	void Update(const float& deltaTime) override;

	void SetLookAtObject(GameObject* lookAtObject);
private:
	void OnKeyDown(const unsigned char key);
	void OnKeyUp(const unsigned char key);
	void UpdateRotation();
	void UpdatePosition(const float& deltaTime);

	Vector3D movementDirection;
	Vector3D lookDirection;

	Camera* camera;
	GameObject* lookAtObject;
	float sensitivity;

	unsigned char forwardKey = 'W';
	unsigned char backwardKey = 'S';
	unsigned char leftKey = 'A';
	unsigned char rightKey = 'D';
	unsigned char upKey = 'E';
	unsigned char downKey = 'Q';
};

