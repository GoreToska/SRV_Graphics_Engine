#include "OrbitalCameraComponent.h"
#include "../../Input/Keyboard/Keyboard.h"

#include <iostream>

OrbitalCameraComponent::OrbitalCameraComponent(Camera* camera, GameObject* lookAtObject, float sensitivity) :
	camera(camera), lookAtObject(lookAtObject), sensitivity(sensitivity)
{
	Keyboard::GetInstance().KeyPressedEvent.AddRaw(this, &OrbitalCameraComponent::OnKeyDown);
	Keyboard::GetInstance().KeyReleasedEvent.AddRaw(this, &OrbitalCameraComponent::OnKeyUp);
}

void OrbitalCameraComponent::Update(const float& deltaTime)
{
	if (!IsEnabled())
		return;

	UpdatePosition(deltaTime);
	UpdateRotation();
}

void OrbitalCameraComponent::OnKeyDown(const unsigned char key)
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

void OrbitalCameraComponent::OnKeyUp(const unsigned char key)
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

void OrbitalCameraComponent::UpdateRotation()
{
	camera->SetLookAtPosition(lookAtObject->GetTransform()->GetPosition());
}

void OrbitalCameraComponent::UpdatePosition(const float& deltaTime)
{
	Vector3D forward = camera->GetForwardVector() * movementDirection.z;
	Vector3D right = camera->GetRightVector() * movementDirection.x;
	Vector3D up = Vector3D(0, movementDirection.y, 0);

	camera->AddPosition((right + forward + up) * deltaTime * sensitivity);
}
