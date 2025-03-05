#include "CameraMovementComponent.h"

#include "../../Input/Keyboard/Keyboard.h"
#include "../../Input/Mouse/Mouse.h"
#include <iostream>


CameraMovementComponent::CameraMovementComponent(Camera* camera)
{
	this->camera = camera;

	Keyboard::GetInstance().KeyPressedEvent.AddRaw(this, &CameraMovementComponent::OnKeyDown);
	Keyboard::GetInstance().KeyReleasedEvent.AddRaw(this, &CameraMovementComponent::OnKeyUp);
	Mouse::GetInstance().OnMouseDeltaEvent.AddRaw(this, &CameraMovementComponent::OnMouseMoved);
}

void CameraMovementComponent::Update(const float& deltaTime)
{
	if (!IsEnabled())
		return;

	UpdatePosition(deltaTime);
	UpdateRotation(deltaTime);
}

void CameraMovementComponent::UpdateRotation(const float& deltaTime)
{
	camera->AddRotation(lookDirection * deltaTime * mouseSpeed);
	lookDirection = Vector3D(0, 0, 0);
}

void CameraMovementComponent::UpdatePosition(const float& deltaTime)
{
	Vector3D forward = camera->GetForwardVector() * movementDirection.z;
	Vector3D right = camera->GetRightVector() * movementDirection.x;
	Vector3D up = Vector3D(0, movementDirection.y, 0);

	camera->AddPosition((right + forward + up) * deltaTime * moveSpeed);
}

void CameraMovementComponent::OnMouseMoved(const int x, const int y)
{
	lookDirection = Vector3D(y, x, 0);
}

void CameraMovementComponent::OnKeyDown(const unsigned char key)
{
	if (key == forwardKey)
	{
		movementDirection += Vector3D(0, 0, 1);
		return;
	}

	if (key == backwardKey)
	{
		movementDirection += Vector3D(0, 0, -1);
		return;
	}

	if (key == rightKey)
	{
		movementDirection += Vector3D(1, 0, 0);
		return;
	}

	if (key == leftKey)
	{
		movementDirection += Vector3D(-1, 0, 0);
		return;
	}

	if (key == upKey)
	{
		movementDirection += Vector3D(0, 1, 0);
		return;
	}

	if (key == downKey)
	{
		movementDirection += Vector3D(0, -1, 0);
		return;
	}
}

void CameraMovementComponent::OnKeyUp(const unsigned char key)
{
	if (key == forwardKey)
	{
		movementDirection += Vector3D(0, 0, -1);
		return;
	}

	if (key == backwardKey)
	{
		movementDirection += Vector3D(0, 0, 1);
		return;
	}

	if (key == rightKey)
	{
		movementDirection += Vector3D(-1, 0, 0);
		return;
	}

	if (key == leftKey)
	{
		movementDirection += Vector3D(1, 0, 0);
		return;
	}

	if (key == upKey)
	{
		movementDirection += Vector3D(0, -1, 0);
		return;
	}

	if (key == downKey)
	{
		movementDirection += Vector3D(0, 1, 0);
		return;
	}
}
