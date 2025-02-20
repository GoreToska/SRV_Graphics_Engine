#pragma once
#include <iostream>
#include <typeinfo>

#include "../Graphics/RenderObjects/RenderComponent.h"


class PongInput
{
private:
	unsigned char upKey;
	unsigned char downKey;
	RenderComponent* gameObject = nullptr;

public:
	void SetInput(unsigned char upKey, unsigned char downKey)
	{
		this->upKey = upKey;
		this->downKey = downKey;
	}

	void SetInputTarget(RenderComponent* gameObject)
	{
		this->gameObject = gameObject;
	}

	void PlayerInput(const unsigned char key)
	{
		if (key == upKey)
		{
			gameObject->MovePosition({ 0.0, 0.05, 0 });
			std::cout << gameObject->id << std::endl;
		}

		if (key == downKey)
		{
			gameObject->MovePosition({ 0.0, -0.05, 1 });
		}
	}
};

