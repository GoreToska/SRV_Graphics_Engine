#pragma once


class IComponent
{
public:
	virtual void Update(const float& deltaTime) = 0;
};