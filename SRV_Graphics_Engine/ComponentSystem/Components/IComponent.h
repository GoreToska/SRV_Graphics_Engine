#pragma once


class IComponent
{
public:
	virtual void Update(float deltaTime) = 0;
};