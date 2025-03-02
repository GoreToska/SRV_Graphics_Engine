#include "PongInputComponent.h"
#include "../../Input/Keyboard/Keyboard.h"
#include <iostream>

PongInputComponent::PongInputComponent(GameObject* posessedGameObject, float size)
{
	posessedObjectTransform = posessedGameObject->GetTransform();

	this->size = size;

	Keyboard::GetInstance().KeyPressedEvent.AddRaw(this, &PongInputComponent::OnKeyDown);
	Keyboard::GetInstance().KeyReleasedEvent.AddRaw(this, &PongInputComponent::OnKeyUp);
}

void PongInputComponent::Update(float deltaTime)
{
	float signedSize = size;
	int side = 1;

	if (movementVector.y < 0)
	{
		signedSize *= -1;
		side *= -1;
	}

	if (side > 0)
	{
		if (posessedObjectTransform->GetPosition().y + movementVector.y + signedSize / 2 < side)
			posessedObjectTransform->MovePosition(movementVector * deltaTime);
	}
	else
	{
		if (posessedObjectTransform->GetPosition().y + movementVector.y + signedSize / 2 > side)
			posessedObjectTransform->MovePosition(movementVector * deltaTime);
	}

}

void PongInputComponent::SetInput(unsigned char upKey, unsigned char downKey)
{
	up = upKey;
	down = downKey;
}

void PongInputComponent::SetSpeed(float value)
{
	movementSpeed = value;
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
