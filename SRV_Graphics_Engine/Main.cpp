#pragma region Includes
#include <Windows.h>
#include <iostream>
#include <tuple>

#include "./Utils/Logger.h"
#include "./Engine/Engine.h"

#include "./Input/Keyboard/Keyboard.h"
#include"./Shapes/Shapes.h"
#include "ComponentSystem/GameObject.h"
#include "ComponentSystem/Components/CollisionComponent.h"  
#include "ComponentSystem/Components/RotationComponent.h"  
#include "ComponentSystem/Components/CameraMovementComponent.h"  
#include "Games/Planets/OrbitalMovementComponent.h"
#include "Games/Planets/OrbitalCameraComponent.h"
#include "DataTypes/ColorRGB.h"
#include "Games/Planets/PlanetCameraSwitchComponent.h"

#pragma endregion

int main()
{
	std::string applicationName = "Pong";
	std::string windowClass = "WindowClass";
	HINSTANCE hInstance = GetModuleHandle(nullptr);

	SRVEngine.Initialize(hInstance, applicationName, windowClass, 1024, 768);

	auto cubeShape = Shapes::GetCubeShape(1, WHITE);
	auto sunShape = Shapes::GetSphereShape(1, 16, 16, YELLOW);
	auto mercuryShape = Shapes::GetSphereShape(1, 16, 16, GRAY);
	auto venusShape = Shapes::GetSphereShape(1, 16, 16, LIGHT_YELLOW);
	auto earthShape = Shapes::GetSphereShape(1, 16, 16, GRASS_GREEN);
	auto marsShape = Shapes::GetSphereShape(1, 16, 16, SCARLET_RED);
	auto jupiterShape = Shapes::GetSphereShape(1, 16, 16, LIGHT_ORANGE);
	auto saturnShape = Shapes::GetSphereShape(1, 16, 16, LIGHT_YELLOW_2);
	auto uranusShape = Shapes::GetSphereShape(1, 16, 16, BlUE_GREEN);
	auto neptuneShape = Shapes::GetSphereShape(1, 16, 16, PERSID_BlUE);
	auto plutoShape = Shapes::GetCubeShape(1, LIGHT_BROWN);

	GameObject* Sun = new GameObject(Vector3D(0.0f, 0.0f, 0.0f));
	Sun->AddComponent(new RotationComponent(Sun->GetTransform(), Vector3D(0, 1, 0), 0.05));
	Sun->AddComponent(new RenderComponent(Sun->GetTransform(), std::get<0>(sunShape), std::get<1>(sunShape)));
	SRVEngine.AddGameObject(Sun);

	GameObject* Mercury = new GameObject(Vector3D(5.0f, 0.0f, 0.0f));
	Mercury->AddComponent(new RotationComponent(Mercury->GetTransform(), Vector3D(0, 1, 0), 0.05));
	Mercury->GetTransform()->SetScale(Vector3D(0.1, 0.1, 0.1));
	Mercury->AddComponent(new RenderComponent(Mercury->GetTransform(), std::get<0>(mercuryShape), std::get<1>(mercuryShape)));
	Mercury->AddComponent(new OrbitalMovementComponent(Mercury, Sun, Vector3D(0, 1, 0), 0.001f, 5.0f));
	SRVEngine.AddGameObject(Mercury);

	GameObject* Venus = new GameObject(Vector3D(10.0f, 0.0f, 0.0f));
	Venus->AddComponent(new RotationComponent(Venus->GetTransform(), Vector3D(0, 1, 0), 0.05));
	Venus->GetTransform()->SetScale(Vector3D(0.2, 0.2, 0.2));
	Venus->AddComponent(new RenderComponent(Venus->GetTransform(), std::get<0>(venusShape), std::get<1>(venusShape)));
	Venus->AddComponent(new OrbitalMovementComponent(Venus, Sun, Vector3D(0, 1, 0), 0.0003f, 10.0f));
	SRVEngine.AddGameObject(Venus);

	GameObject* Earth = new GameObject(Vector3D(15.0f, 0.0f, 0.0f));
	Earth->AddComponent(new RotationComponent(Earth->GetTransform(), Vector3D(0, 1, 0), 5));
	Earth->GetTransform()->SetScale(Vector3D(0.25, 0.25, 0.25));
	Earth->AddComponent(new RenderComponent(Earth->GetTransform(), std::get<0>(earthShape), std::get<1>(earthShape)));
	Earth->AddComponent(new OrbitalMovementComponent(Earth, Sun, Vector3D(0, 1, 0), 0.0002f, 15.0f));
	SRVEngine.AddGameObject(Earth);

	GameObject* Moon = new GameObject(Vector3D(18.0f, 0.0f, 0.0f));
	Moon->AddComponent(new RotationComponent(Moon->GetTransform(), Vector3D(0, 1, 0), 0.05));
	Moon->GetTransform()->SetScale(Vector3D(0.15, 0.15, 0.15));
	Moon->AddComponent(new RenderComponent(Moon->GetTransform(), std::get<0>(mercuryShape), std::get<1>(mercuryShape)));
	Moon->AddComponent(new OrbitalMovementComponent(Moon, Earth, Vector3D(0, 1, 1), 0.005f, 1.5f));
	SRVEngine.AddGameObject(Moon);

	GameObject* Mars = new GameObject(Vector3D(20.0f, 0.0f, 0.0f));
	Mars->AddComponent(new RotationComponent(Mars->GetTransform(), Vector3D(0, 1, 0), 0.05));
	Mars->GetTransform()->SetScale(Vector3D(0.25, 0.25, 0.25));
	Mars->AddComponent(new RenderComponent(Mars->GetTransform(), std::get<0>(marsShape), std::get<1>(marsShape)));
	Mars->AddComponent(new OrbitalMovementComponent(Mars, Sun, Vector3D(0, 1, 0), 0.00015f, 20.0f));
	SRVEngine.AddGameObject(Mars);

	GameObject* Jupiter = new GameObject(Vector3D(25.0f, 0.0f, 0.0f));
	Jupiter->AddComponent(new RotationComponent(Jupiter->GetTransform(), Vector3D(0, 1, 0), 0.05));
	Jupiter->GetTransform()->SetScale(Vector3D(0.8, 0.8, 0.8));
	Jupiter->AddComponent(new RenderComponent(Jupiter->GetTransform(), std::get<0>(jupiterShape), std::get<1>(jupiterShape)));
	Jupiter->AddComponent(new OrbitalMovementComponent(Jupiter, Sun, Vector3D(0, 1, 0), 0.0001f, 25.0f));
	SRVEngine.AddGameObject(Jupiter);

	GameObject* Saturn = new GameObject(Vector3D(30.0f, 0.0f, 0.0f));
	Saturn->AddComponent(new RotationComponent(Saturn->GetTransform(), Vector3D(0, 1, 0), 0.05));
	Saturn->GetTransform()->SetScale(Vector3D(0.6, 0.6, 0.6));
	Saturn->AddComponent(new RenderComponent(Saturn->GetTransform(), std::get<0>(saturnShape), std::get<1>(saturnShape)));
	Saturn->AddComponent(new OrbitalMovementComponent(Saturn, Sun, Vector3D(0, 1, 0), 0.00015f, 30.0f));
	SRVEngine.AddGameObject(Saturn);

	GameObject* SaturnMoon1 = new GameObject(Vector3D(0.0f, 0.0f, 0.0f));
	SaturnMoon1->AddComponent(new RotationComponent(SaturnMoon1->GetTransform(), Vector3D(0, 1, 0), 0.05));
	SaturnMoon1->GetTransform()->SetScale(Vector3D(0.2, 0.2, 0.2));
	SaturnMoon1->AddComponent(new RenderComponent(SaturnMoon1->GetTransform(), std::get<0>(marsShape), std::get<1>(marsShape)));
	SaturnMoon1->AddComponent(new OrbitalMovementComponent(SaturnMoon1, Saturn, Vector3D(1, 1, 0), -0.001f, 2.0f));
	SRVEngine.AddGameObject(SaturnMoon1);

	GameObject* SaturnMoon2 = new GameObject(Vector3D(0.0f, 0.0f, 0.0f));
	SaturnMoon2->AddComponent(new RotationComponent(SaturnMoon2->GetTransform(), Vector3D(0, 1, 0), 0.05));
	SaturnMoon2->GetTransform()->SetScale(Vector3D(0.25, 0.25, 0.25));
	SaturnMoon2->AddComponent(new RenderComponent(SaturnMoon2->GetTransform(), std::get<0>(cubeShape), std::get<1>(cubeShape)));
	SaturnMoon2->AddComponent(new OrbitalMovementComponent(SaturnMoon2, Saturn, Vector3D(0, 1, 1), 0.001f, 4.0f));
	SRVEngine.AddGameObject(SaturnMoon2);

	GameObject* Uranus = new GameObject(Vector3D(35.0f, 0.0f, 0.0f));
	Uranus->AddComponent(new RotationComponent(Uranus->GetTransform(), Vector3D(0, 1, 0), 0.05));
	Uranus->GetTransform()->SetScale(Vector3D(0.4, 0.4, 0.4));
	Uranus->AddComponent(new RenderComponent(Uranus->GetTransform(), std::get<0>(uranusShape), std::get<1>(uranusShape)));
	Uranus->AddComponent(new OrbitalMovementComponent(Uranus, Sun, Vector3D(0, 1, 0), 0.0002f, 35.0f));
	SRVEngine.AddGameObject(Uranus);

	GameObject* Neptune = new GameObject(Vector3D(40.0f, 0.0f, 0.0f));
	Neptune->AddComponent(new RotationComponent(Neptune->GetTransform(), Vector3D(0, 1, 0), 0.05));
	Neptune->GetTransform()->SetScale(Vector3D(0.36, 0.36, 0.36));
	Neptune->AddComponent(new RenderComponent(Neptune->GetTransform(), std::get<0>(neptuneShape), std::get<1>(neptuneShape)));
	Neptune->AddComponent(new OrbitalMovementComponent(Neptune, Sun, Vector3D(0, 1, 0), 0.00025f, 40.0f));
	SRVEngine.AddGameObject(Neptune);

	GameObject* Pluto = new GameObject(Vector3D(40.0f, 0.0f, 0.0f));
	Pluto->AddComponent(new RotationComponent(Pluto->GetTransform(), Vector3D(0, 1, 1), 0.1));
	Pluto->GetTransform()->SetScale(Vector3D(0.7, 0.7, 0.7));
	Pluto->AddComponent(new RenderComponent(Pluto->GetTransform(), std::get<0>(plutoShape), std::get<1>(plutoShape)));
	Pluto->AddComponent(new OrbitalMovementComponent(Pluto, Sun, Vector3D(0, 1, 0), 0.00004f, 45.0f));
	SRVEngine.AddGameObject(Pluto);

	GameObject* camera = new GameObject();
	camera->AddComponent(new CameraMovementComponent(SRVEngine.GetGraphics().GetCamera()));
	camera->AddComponent(new OrbitalCameraComponent(SRVEngine.GetGraphics().GetCamera(), Sun, 0.005));
	camera->GetComponent<CameraMovementComponent>()->SetEnabled(false);
	camera->GetComponent<OrbitalCameraComponent>()->SetEnabled(true);
	camera->AddComponent(new PlanetCameraSwitchComponent(SRVEngine.GetGraphics().GetCamera(), camera->GetComponent<OrbitalCameraComponent>()));
	PlanetCameraSwitchComponent* cameraSwitch = camera->GetComponent<PlanetCameraSwitchComponent>();
	cameraSwitch->AddPlanet(Sun);
	cameraSwitch->AddPlanet(Earth);
	cameraSwitch->AddPlanet(Pluto);
	cameraSwitch->AddPlanet(Saturn);


	SRVEngine.GetGraphics().GetCamera()->SetOrthographicProjection(80, 60, 0.1, 1000);
	SRVEngine.AddGameObject(camera);

	while (SRVEngine.ProcessMessages())
	{
		float deltaTime = SRVEngine.GetTimer()->GetMilisecondsElapsed();
		SRVEngine.GetTimer()->Restart();

		SRVEngine.Update(deltaTime);
		SRVEngine.RenderFrame();
	}
}

