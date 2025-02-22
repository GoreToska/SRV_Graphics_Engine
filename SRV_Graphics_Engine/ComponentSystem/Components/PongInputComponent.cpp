#include "PongInputComponent.h"
#include "../../Input/Keyboard/Keyboard.h"
#include <iostream>

PongInputComponent::PongInputComponent(GameObject* posessedGameObject)
{
	posessedObjectTransform = posessedGameObject->GetTransform();

	Keyboard::GetInstance().KeyPressedEvent.AddRaw(this, &PongInputComponent::OnKeyDown);
	Keyboard::GetInstance().KeyReleasedEvent.AddRaw(this, &PongInputComponent::OnKeyUp);
}

void PongInputComponent::Update()
{
	posessedObjectTransform->MovePosition(movementVector);
}

IComponent::ComponentType PongInputComponent::GetType()
{
	return InputComponentType;
}

void PongInputComponent::SetInput(unsigned char upKey, unsigned char downKey)
{
	up = upKey;
	down = downKey;
}

void PongInputComponent::OnKeyDown(const unsigned char key)
{
	if (key == up)
	{
		movementVector += Vector3D(0, movementSpeed, 0);
	}

	if (key == down)
	{
		movementVector += Vector3D(0, -movementSpeed, 0);
	}
}

void PongInputComponent::OnKeyUp(const unsigned char key)
{
	if (key == up)
	{
		movementVector -= Vector3D(0, movementSpeed, 0);
	}

	if (key == down)
	{
		movementVector -= Vector3D(0, -movementSpeed, 0);
	}
}
