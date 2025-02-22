#pragma once

#include "./IComponent.h"

class BoxCollisionComponent2D : public IComponent
{
public:
	void Update() override;
	ComponentType GetType() override;
};

