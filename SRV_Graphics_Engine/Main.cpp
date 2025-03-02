#pragma region Includes
#include <Windows.h>
#include <iostream>

#include "./Utils/Logger.h"
#include "./Engine/Engine.h"

#include "./Input/Keyboard/Keyboard.h"
#include"./Shapes/Shapes.h"
#include "ComponentSystem/GameObject.h"
#include "ComponentSystem/Components/CollisionComponent.h"  
#include "ComponentSystem/Components/CameraMovementComponent.h"  
#include "Games/Pong/PongSpawnerComponent.h"
#include "Games/Pong/PongGameMode.h"
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

	auto cubeShape = Shapes::GetCubeShape(1);
	auto sphereShape = Shapes::GetSphereShape(1, 16, 16, LIGHT_GRAY);

	GameObject* cube1 = new GameObject(Vector3D(1, 1, 1));
	cube1->AddComponent(new RenderComponent(cube1->GetTransform(), std::get<0>(cubeShape), std::get<1>(cubeShape)));
	SRVEngine.AddGameObject(cube1);

	GameObject* cube2 = new GameObject(Vector3D(-1, -1, -1));
	cube2->AddComponent(new RenderComponent(cube2->GetTransform(), std::get<0>(cubeShape), std::get<1>(cubeShape)));
	SRVEngine.AddGameObject(cube2);

	GameObject* sphere1 = new GameObject(Vector3D(3.0f, 3.0f, 3.0f));
	sphere1->AddComponent(new RenderComponent(sphere1->GetTransform(), std::get<0>(sphereShape), std::get<1>(sphereShape)));
	SRVEngine.AddGameObject(sphere1);

	while (SRVEngine.ProcessMessages())
	{
		SRVEngine.Update();
		SRVEngine.RenderFrame();
	}
}

