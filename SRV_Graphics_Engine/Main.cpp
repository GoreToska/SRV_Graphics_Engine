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
#include "ComponentSystem/Components/Collision/CollisionComponent.h"  
#include "Games/Katamari/KatamariMovementComponent.h"
#include "ComponentSystem/Components/Camera/TopDownCameraComponent.h"

#pragma endregion

int main()
{
	std::string applicationName = "Pong";
	std::string windowClass = "WindowClass";
	HINSTANCE hInstance = GetModuleHandle(nullptr);

	SRVEngine.Initialize(hInstance, applicationName, windowClass, 1024, 768);

	GameObject* redBird2 = new GameObject(Vector3D(5.0f, 0.0f, 0.0f));
	std::string file = "Data\\Models\\RedBird\\Angry_Bird.obj";

	redBird2->AddComponent(new MeshRendererComponent(file, redBird2));
	redBird2->GetTransform()->SetScale(Vector3D(0.01, 0.01, 0.01));
	SRVEngine.AddGameObject(redBird2);

	GameObject* redBird = new GameObject(Vector3D(0.0f, 0.0f, 0.0f));

	redBird->AddComponent(new MeshRendererComponent(file, redBird));
	redBird->AddComponent(new KatamariMovementComponent(redBird));
	redBird->GetTransform()->SetScale(Vector3D(0.01, 0.01, 0.01));
	redBird->GetTransform()->AddLocalRotation(Vector3D(1,0,0), 45);
	SRVEngine.AddGameObject(redBird);

	GameObject* camera = new GameObject();
	camera->AddComponent(new TopDownCameraComponent(SRVEngine.GetGraphics().GetCamera(), redBird, Vector3D(0, 20, 0)));
	SRVEngine.AddGameObject(camera);

	while (SRVEngine.ProcessMessages())
	{
		float deltaTime = SRVEngine.GetTimer()->GetMilisecondsElapsed();
		SRVEngine.GetTimer()->Restart();

		SRVEngine.Update(deltaTime);
		SRVEngine.RenderFrame();
	}
}

