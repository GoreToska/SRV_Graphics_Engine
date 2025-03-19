#include "KatamariMovementComponent.h"
#include "../../Input/Keyboard/Keyboard.h"

#include <cmath> 
#include "../../Engine/Engine.h"

KatamariMovementComponent::KatamariMovementComponent(GameObject* gameObject) :
	gameObject(gameObject)
{
	Keyboard::GetInstance().KeyPressedEvent.AddRaw(this, &KatamariMovementComponent::OnKeyPressed);
	Keyboard::GetInstance().KeyReleasedEvent.AddRaw(this, &KatamariMovementComponent::OnKeyReleased);
}

void KatamariMovementComponent::Update(const float& deltaTime)
{
	if (!IsEnabled())
		return;

	UpdatePosition(deltaTime);
	UpdateRotation(deltaTime);
	UpdateHeight(deltaTime);
}

void KatamariMovementComponent::OnKeyPressed(const unsigned char key)
{
	if (key == upKey)
	{
		movementDirection += Vector3D(0, 0, 1);
		return;
	}

	if (key == downKey)
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
}

void KatamariMovementComponent::OnKeyReleased(const unsigned char key)
{
	if (key == upKey)
	{
		movementDirection -= Vector3D(0, 0, 1);
		return;
	}

	if (key == downKey)
	{
		movementDirection -= Vector3D(0, 0, -1);
		return;
	}

	if (key == rightKey)
	{
		movementDirection -= Vector3D(1, 0, 0);
		return;
	}

	if (key == leftKey)
	{
		movementDirection -= Vector3D(-1, 0, 0);
		return;
	}
}

void KatamariMovementComponent::UpdateRotation(const float& deltaTime)
{
	Vector3D cameraRelativeDirection = CalculateMovementDirection();
	Vector3D rotationAxis = { cameraRelativeDirection.z, 0, -cameraRelativeDirection.x };
	gameObject->GetTransform()->AddWorldRotation(rotationAxis, deltaTime * speed * 20);
}

void KatamariMovementComponent::UpdateHeight(const float& deltaTime)
{
	Vector3D position = gameObject->GetTransform()->GetPosition();

	float landscapeHeight = GetLandscapeHeight(position.x, position.z) + 1;
	position.y = landscapeHeight;

	gameObject->GetTransform()->SetPosition(position);
}

void KatamariMovementComponent::UpdatePosition(const float& deltaTime)
{
	Vector3D cameraRelativeDirection = CalculateMovementDirection();
	gameObject->GetTransform()->MovePosition(cameraRelativeDirection * deltaTime * speed);
}

float KatamariMovementComponent::GetLandscapeHeight(float x, float z)
{
	float amplitude = 1.5f;
	float frequency = 0.5f;
	return amplitude * std::sin(x * frequency) * std::cos(z * frequency);
}

Vector3D KatamariMovementComponent::CalculateMovementDirection()
{
	Vector3D normalizedForward = SRVEngine.GetGraphics().GetCamera()->GetForwardVector().Normalized();
	Vector3D normalizedRight = SRVEngine.GetGraphics().GetCamera()->GetRightVector().Normalized();

	Vector3D newDirection = (normalizedForward * movementDirection.z) + (normalizedRight * movementDirection.x);
	return newDirection.Normalized(); 
}
