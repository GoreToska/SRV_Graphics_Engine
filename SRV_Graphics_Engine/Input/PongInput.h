#pragma once
#include <iostream>
#include <typeinfo>



class PongInput
{
private:
	unsigned char upKey;
	unsigned char downKey;
	GameObject* gameObject = nullptr;

public:
	void SetInput(unsigned char upKey, unsigned char downKey)
	{
		this->upKey = upKey;
		this->downKey = downKey;
	}

	void SetInputTarget(GameObject* gameObject)
	{
		this->gameObject = gameObject;
	}

	void PlayerInput(const unsigned char key)
	{
		if (key == upKey)
		{
			gameObject->GetTransform()->MovePosition({ 0.0, 0.05, 0 });
		}

		if (key == downKey)
		{
			gameObject->GetTransform()->MovePosition({ 0.0, -0.05, 0 });
		}
	}
};

