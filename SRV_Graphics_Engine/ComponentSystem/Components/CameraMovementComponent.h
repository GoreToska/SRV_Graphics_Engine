#pragma once

#include "../Components/IComponent.h"
#include "../../DataTypes/Vector3D.h"
#include "../../Graphics/Camera.h"

class CameraMovementComponent : public IComponent
{
public:
	CameraMovementComponent(Camera* camera);

	void Update(const float& deltaTime) override;

private:
	void UpdateRotation(const float& deltaTime);
	void UpdatePosition(const float& deltaTime);
	void OnMouseMoved(const int x, const int y);
	void OnKeyDown(const unsigned char key);
	void OnKeyUp(const unsigned char key);

	float moveSpeed = 0.01;
	float mouseSpeed = 0.001;

	Vector3D movementDirection;
	Vector3D lookDirection;

	Camera* camera;

	unsigned char forwardKey = 'W';
	unsigned char backwardKey = 'S';
	unsigned char leftKey = 'A';
	unsigned char rightKey = 'D';
	unsigned char upKey = 'E';
	unsigned char downKey = 'Q';
};