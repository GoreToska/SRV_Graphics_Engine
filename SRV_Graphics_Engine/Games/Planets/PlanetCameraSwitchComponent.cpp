#include "PlanetCameraSwitchComponent.h"
#include "../../Input/Keyboard/Keyboard.h"

PlanetCameraSwitchComponent::PlanetCameraSwitchComponent(Camera* camera) : camera(camera)
{
	Keyboard::GetInstance().KeyPressedEvent.AddRaw(this, &PlanetCameraSwitchComponent::OnKeyPressed);
}

void PlanetCameraSwitchComponent::Update(const float& deltaTime)
{
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
}
