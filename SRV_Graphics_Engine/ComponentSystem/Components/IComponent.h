#pragma once


class IComponent
{
public:
	enum ComponentType
	{
		RenderComponentType,
		TransfromComponentType,
		CollisionComponentType,
		InputComponentType,
	};

	virtual void Update() = 0;
	virtual ComponentType GetType() = 0;
};