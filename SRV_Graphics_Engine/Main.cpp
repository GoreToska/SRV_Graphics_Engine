#pragma region Includes

#include <Windows.h>
#include <iostream>
#include <tuple>
#include <filesystem>

#include "./Utils/Logger.h"
#include "./Engine/Engine.h"

#include "./Input/Keyboard/Keyboard.h"
#include"./Shapes/Shapes.h"
#include "ComponentSystem/GameObject.h"
#include "ComponentSystem/Components/Collision/SphereCollisionComponent.h"  
#include "Games/Katamari/KatamariMovementComponent.h"
#include "Games/Katamari/KatamariCollisionComponent.h"
#include "ComponentSystem/Components/Camera/TopDownCameraComponent.h"
#include "DataTypes/ModelData.h"

#pragma endregion

int main()
{
	std::string applicationName = "Pong";
	std::string windowClass = "WindowClass";
	HINSTANCE hInstance = GetModuleHandle(nullptr);

	SRVEngine.Initialize(hInstance, applicationName, windowClass, 1024, 768);

	ModelData redBirdModelData = { "Data\\Models\\RedBird\\RedBird.obj", L"Data\\Models\\RedBird\\RedBird.png" };
	ModelData blueBirdModelData = { "Data\\Models\\BlueBird\\BlueBird.obj", L"Data\\Models\\BlueBird\\BlueBird.png" };
	ModelData greenPigModelData = { "Data\\Models\\GreenPig\\GreenPig.obj", L"Data\\Models\\GreenPig\\GreenPig.png" };
	ModelData amongusModelData = { "Data\\Models\\Amongus\\Amongus.obj", L"Data\\Models\\Amongus\\Amongus.jpg" };
	ModelData octopusModelData = { "Data\\Models\\Octopus\\Octopus.obj", L"Data\\Models\\Octopus\\Octopus.png" };
	ModelData kettleModelData = { "Data\\Models\\Kettle\\Kettle.obj", L"Data\\Models\\Kettle\\Kettle.png" };
	ModelData memoryCardModelData = { "Data\\Models\\MemoryCard\\MemoryCard.obj", L"Data\\Models\\MemoryCard\\MemoryCard.jpg" };
	ModelData radioModelData = { "Data\\Models\\Radio\\Radio.obj", L"Data\\Models\\Radio\\Radio.png" };
	ModelData phoneModelData = { "Data\\Models\\Phone\\Phone.obj", L"Data\\Models\\Phone\\Phone.png" };
	ModelData hatModelData = { "Data\\Models\\Hat\\Hat.obj", L"Data\\Models\\Hat\\Hat.png" };


	GameObject* blueBird = new GameObject(Vector3D(5.0f, 0.0f, 0.0f));
	blueBird->AddComponent(new MeshRendererComponent(blueBirdModelData, blueBird));
	blueBird->AddComponent(new SphereCollisionComponent(blueBird, Vector3D(0.0f, 0.0f, 0.0f), 2.4));
	blueBird->GetTransform()->SetScale(Vector3D(0.02, 0.02, 0.02));
	SRVEngine.AddGameObject(blueBird);

	GameObject* phone = new GameObject(Vector3D(-25.0f, 0.0f, 5.0f));
	phone->AddComponent(new MeshRendererComponent(phoneModelData, phone));
	phone->AddComponent(new SphereCollisionComponent(phone, Vector3D(0.0f, 0.0f, 0.0f), 3));
	phone->GetTransform()->SetScale(Vector3D(0.04, 0.04, 0.04));
	SRVEngine.AddGameObject(phone);

	GameObject* hat = new GameObject(Vector3D(-25.0f, 0.0f, 20.0f));
	hat->AddComponent(new MeshRendererComponent(hatModelData, hat));
	hat->AddComponent(new SphereCollisionComponent(hat, Vector3D(0.0f, 0.0f, 0.0f), 2));
	hat->GetTransform()->SetScale(Vector3D(0.2, 0.2, 0.2));
	SRVEngine.AddGameObject(hat);

	GameObject* memoryCard = new GameObject(Vector3D(-20.0f, 0.0f, -20.0f));
	memoryCard->AddComponent(new MeshRendererComponent(memoryCardModelData, memoryCard));
	memoryCard->AddComponent(new SphereCollisionComponent(memoryCard, Vector3D(0.0f, 0.0f, 0.0f), 3));
	memoryCard->GetTransform()->SetScale(Vector3D(1, 1, 1));
	SRVEngine.AddGameObject(memoryCard);

	GameObject* radio = new GameObject(Vector3D(-20.0f, 0.0f, 10.0f));
	radio->AddComponent(new MeshRendererComponent(radioModelData, radio));
	radio->AddComponent(new SphereCollisionComponent(radio, Vector3D(0.0f, 0.0f, 0.0f), 2.7));
	radio->GetTransform()->SetScale(Vector3D(1, 1, 1));
	SRVEngine.AddGameObject(radio);

	GameObject* kettle = new GameObject(Vector3D(-15.0f, 0.0f, 10.0f));
	kettle->AddComponent(new MeshRendererComponent(kettleModelData, kettle));
	kettle->AddComponent(new SphereCollisionComponent(kettle, Vector3D(0.0f, 0.0f, 0.0f), 3));
	kettle->GetTransform()->SetScale(Vector3D(0.2, 0.2, 0.2));
	SRVEngine.AddGameObject(kettle);

	GameObject* octopus = new GameObject(Vector3D(5.0f, 0.0f, -10.0f));
	octopus->AddComponent(new MeshRendererComponent(octopusModelData, octopus));
	octopus->AddComponent(new SphereCollisionComponent(octopus, Vector3D(0.0f, 0.0f, 0.0f), 4));
	octopus->GetTransform()->SetScale(Vector3D(3, 3, 3));
	SRVEngine.AddGameObject(octopus);

	GameObject* amongus = new GameObject(Vector3D(10.0f, 0.0f, 5.0f));
	amongus->AddComponent(new MeshRendererComponent(amongusModelData, amongus));
	amongus->AddComponent(new SphereCollisionComponent(amongus, Vector3D(0.0f, 0.0f, 0.0f), 2));
	amongus->GetTransform()->SetScale(Vector3D(0.03, 0.03, 0.03));
	SRVEngine.AddGameObject(amongus);

	GameObject* greenPig = new GameObject(Vector3D(5.0f, 0.0f, 5.0f));
	greenPig->AddComponent(new MeshRendererComponent(greenPigModelData, greenPig));
	greenPig->AddComponent(new SphereCollisionComponent(greenPig, Vector3D(0.0f, 0.0f, 0.0f), 2));
	greenPig->GetTransform()->SetScale(Vector3D(0.02, 0.02, 0.02));
	SRVEngine.AddGameObject(greenPig);

	GameObject* redBird = new GameObject(Vector3D(0.0f, 0.0f, 0.0f));
	redBird->AddComponent(new MeshRendererComponent(redBirdModelData, redBird));
	redBird->AddComponent(new SphereCollisionComponent(redBird, Vector3D(0, 0, 0), 2.2));
	redBird->AddComponent(new KatamariMovementComponent(redBird));
	redBird->GetTransform()->SetScale(Vector3D(0.01, 0.01, 0.01));
	redBird->AddComponent(new KatamariCollisionComponent(redBird));
	SRVEngine.AddGameObject(redBird);

	GameObject* camera = new GameObject();
	camera->AddComponent(new TopDownCameraComponent(SRVEngine.GetGraphics().GetCamera(), redBird, Vector3D(0, 20, -10)));
	SRVEngine.AddGameObject(camera);

	while (SRVEngine.ProcessMessages())
	{
		float deltaTime = SRVEngine.GetTimer()->GetMilisecondsElapsed();
		SRVEngine.GetTimer()->Restart();

		SRVEngine.Update(deltaTime);
		SRVEngine.RenderFrame();
	}
}

