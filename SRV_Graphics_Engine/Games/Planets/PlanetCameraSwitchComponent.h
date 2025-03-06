#pragma once

#include "../../ComponentSystem/Components/IComponent.h"
#include "../../Graphics/Camera.h"
#include "../../ComponentSystem/GameObject.h"
#include "OrbitalCameraComponent.h"

class PlanetCameraSwitchComponent : public IComponent
{
public:
	PlanetCameraSwitchComponent(Camera* camera, OrbitalCameraComponent* orbitalComponent);

	void Update(const float& deltaTime) override;

	void AddPlanet(GameObject* gameObject);

private:
	void OnKeyPressed(const unsigned char key);

	const unsigned char PerspectiveKey = '1';
	const unsigned char OrthographicKey = '2';
	const unsigned char PlanetKey = '3';

	std::vector<GameObject*> planets{};
	int currentPlanet = 0;

	Camera* camera;
	OrbitalCameraComponent* orbitalComponent;
};

