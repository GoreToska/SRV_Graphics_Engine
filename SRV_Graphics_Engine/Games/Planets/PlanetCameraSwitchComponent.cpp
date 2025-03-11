#include "PlanetCameraSwitchComponent.h"
#include "../../Input/Keyboard/Keyboard.h"

PlanetCameraSwitchComponent::PlanetCameraSwitchComponent(Camera* camera, OrbitalCameraComponent* orbitalComponent) : camera(camera), orbitalComponent(orbitalComponent)
{
	Keyboard::GetInstance().KeyPressedEvent.AddRaw(this, &PlanetCameraSwitchComponent::OnKeyPressed);
}

void PlanetCameraSwitchComponent::Update(const float& deltaTime)
{
	Vector3D newPos = planets[currentPlanet]->GetTransform()->GetPosition();
	newPos.z -= planets[currentPlanet]->GetTransform()->GetScale().x * 1.4;
	camera->SetPosition(newPos);
}

void PlanetCameraSwitchComponent::AddPlanet(GameObject* gameObject)
{
	planets.push_back(gameObject);
}

void PlanetCameraSwitchComponent::OnKeyPressed(const unsigned char key)
{
	if (key == PerspectiveKey)
	{
		camera->SetPerspectiveProjection();
		return;
	}

	if (key == OrthographicKey)
	{
		camera->SetOrthographicProjection();
		return;
	}

	if (key == PlanetKey)
	{
		++currentPlanet;

		if (currentPlanet >= planets.size())
			currentPlanet = 0;

		orbitalComponent->SetLookAtObject(planets[currentPlanet]);
	}
}
