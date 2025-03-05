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
#include "DataTypes/ColorRGB.h"

#pragma endregion

int main()
{
	std::string applicationName = "Pong";
	std::string windowClass = "WindowClass";
	HINSTANCE hInstance = GetModuleHandle(nullptr);

	SRVEngine.Initialize(hInstance, applicationName, windowClass, 1024, 768);

	GameObject* camera = new GameObject();
	camera->AddComponent(new CameraMovementComponent(SRVEngine.GetGraphics().GetCamera()));
	SRVEngine.AddGameObject(camera);

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
	auto plutoShape = Shapes::GetSphereShape(1, 16, 16, LIGHT_BROWN);


	GameObject* Sun = new GameObject(Vector3D(0.0f, 0.0f, 0.0f));
	Sun->AddComponent(new RotationComponent(Sun->GetTransform(), Vector3D(0, 1, 0), 0.05));
	Sun->AddComponent(new RenderComponent(Sun->GetTransform(), std::get<0>(sunShape), std::get<1>(sunShape)));
	SRVEngine.AddGameObject(Sun);

	GameObject* Mercury = new GameObject(Vector3D(5.0f, 0.0f, 0.0f));
	Mercury->AddComponent(new RotationComponent(Mercury->GetTransform(), Vector3D(0, 1, 0), 0.05));
	Mercury->GetTransform()->SetScale(Vector3D(0.1, 0.1, 0.1));
	Mercury->AddComponent(new RenderComponent(Mercury->GetTransform(), std::get<0>(mercuryShape), std::get<1>(mercuryShape)));
	SRVEngine.AddGameObject(Mercury);

	GameObject* Venus = new GameObject(Vector3D(10.0f, 0.0f, 0.0f));
	Venus->AddComponent(new RotationComponent(Venus->GetTransform(), Vector3D(0, 1, 0), 0.05));
	Venus->GetTransform()->SetScale(Vector3D(0.2, 0.2, 0.2));
	Venus->AddComponent(new RenderComponent(Venus->GetTransform(), std::get<0>(venusShape), std::get<1>(venusShape)));
	SRVEngine.AddGameObject(Venus);

	GameObject* Earth = new GameObject(Vector3D(15.0f, 0.0f, 0.0f));
	Earth->AddComponent(new RotationComponent(Earth->GetTransform(), Vector3D(0, 1, 0), 0.05));
	Earth->GetTransform()->SetScale(Vector3D(0.25, 0.25, 0.25));
	Earth->AddComponent(new RenderComponent(Earth->GetTransform(), std::get<0>(earthShape), std::get<1>(earthShape)));
	SRVEngine.AddGameObject(Earth);

	GameObject* Mars = new GameObject(Vector3D(20.0f, 0.0f, 0.0f));
	Mars->AddComponent(new RotationComponent(Mars->GetTransform(), Vector3D(0, 1, 0), 0.05));
	Mars->GetTransform()->SetScale(Vector3D(0.25, 0.25, 0.25));
	Mars->AddComponent(new RenderComponent(Mars->GetTransform(), std::get<0>(marsShape), std::get<1>(marsShape)));
	SRVEngine.AddGameObject(Mars);

	GameObject* Jupiter = new GameObject(Vector3D(25.0f, 0.0f, 0.0f));
	Jupiter->AddComponent(new RotationComponent(Jupiter->GetTransform(), Vector3D(0, 1, 0), 0.05));
	Jupiter->GetTransform()->SetScale(Vector3D(0.8, 0.8, 0.8));
	Jupiter->AddComponent(new RenderComponent(Jupiter->GetTransform(), std::get<0>(jupiterShape), std::get<1>(jupiterShape)));
	SRVEngine.AddGameObject(Jupiter);

	GameObject* Saturn = new GameObject(Vector3D(30.0f, 0.0f, 0.0f));
	Saturn->AddComponent(new RotationComponent(Saturn->GetTransform(), Vector3D(0, 1, 0), 0.05));
	Saturn->GetTransform()->SetScale(Vector3D(0.6, 0.6, 0.6));
	Saturn->AddComponent(new RenderComponent(Saturn->GetTransform(), std::get<0>(saturnShape), std::get<1>(saturnShape)));
	SRVEngine.AddGameObject(Saturn);

	GameObject* Uranus = new GameObject(Vector3D(35.0f, 0.0f, 0.0f));
	Uranus->AddComponent(new RotationComponent(Uranus->GetTransform(), Vector3D(0, 1, 0), 0.05));
	Uranus->GetTransform()->SetScale(Vector3D(0.4, 0.4, 0.4));
	Uranus->AddComponent(new RenderComponent(Uranus->GetTransform(), std::get<0>(uranusShape), std::get<1>(uranusShape)));
	SRVEngine.AddGameObject(Uranus);

	GameObject* Neptune = new GameObject(Vector3D(40.0f, 0.0f, 0.0f));
	Neptune->AddComponent(new RotationComponent(Neptune->GetTransform(), Vector3D(0, 1, 0), 0.05));
	Neptune->GetTransform()->SetScale(Vector3D(0.36, 0.36, 0.36));
	Neptune->AddComponent(new RenderComponent(Neptune->GetTransform(), std::get<0>(neptuneShape), std::get<1>(neptuneShape)));
	SRVEngine.AddGameObject(Neptune);

	GameObject* Pluto = new GameObject(Vector3D(45.0f, 0.0f, 0.0f));
	Pluto->AddComponent(new RotationComponent(Pluto->GetTransform(), Vector3D(0, 1, 0), 0.05));
	Pluto->GetTransform()->SetScale(Vector3D(0.07, 0.07, 0.07));
	Pluto->AddComponent(new RenderComponent(Pluto->GetTransform(), std::get<0>(plutoShape), std::get<1>(plutoShape)));
	SRVEngine.AddGameObject(Pluto);

	while (SRVEngine.ProcessMessages())
	{
		float deltaTime = SRVEngine.GetTimer()->GetMilisecondsElapsed();
		SRVEngine.GetTimer()->Restart();

		SRVEngine.Update(deltaTime);
		SRVEngine.RenderFrame();
	}
}

