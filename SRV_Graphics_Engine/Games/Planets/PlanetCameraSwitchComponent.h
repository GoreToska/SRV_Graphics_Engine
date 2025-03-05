#pragma once

#include "../../ComponentSystem/Components/IComponent.h"
#include "../../Graphics/Camera.h"

class PlanetCameraSwitchComponent : public IComponent
{
public:
	PlanetCameraSwitchComponent(Camera* camera);

	void Update(const float& deltaTime) override;

private:
	void OnKeyPressed(const unsigned char key);

	const unsigned char PerspectiveKey = '1';
	const unsigned char OrthographicKey = '2';

	Camera* camera;
};

