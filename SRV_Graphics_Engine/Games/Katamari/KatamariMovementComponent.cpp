#include "KatamariMovementComponent.h"
#include "../../Input/Keyboard/Keyboard.h"

#include <cmath> 

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
	Vector3D rotationAxis = { movementDirection.z, 0, -movementDirection.x };
	gameObject->GetTransform()->AddWorldRotation(rotationAxis, deltaTime * speed * 20);
}

void KatamariMovementComponent::UpdateHeight(const float& deltaTime)
{
	// Получаем текущую позицию героя
	Vector3D position = gameObject->GetTransform()->GetPosition();

	// Вычисляем высоту ландшафта в точке (x, z)
	float landscapeHeight = GetLandscapeHeight(position.x, position.z) + 1;

	// Устанавливаем новую высоту героя
	position.y = landscapeHeight;
	gameObject->GetTransform()->SetPosition(position);
}

void KatamariMovementComponent::UpdatePosition(const float& deltaTime)
{
	gameObject->GetTransform()->MovePosition(movementDirection * deltaTime * speed);
}

float KatamariMovementComponent::GetLandscapeHeight(float x, float z)
{
	float amplitude = 1.5f;
	float frequency = 0.5f; 
	return amplitude * std::sin(x * frequency) * std::cos(z * frequency);
}
