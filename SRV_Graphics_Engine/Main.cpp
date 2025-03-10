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
	CoInitialize(NULL);

	std::string applicationName = "Pong";
	std::string windowClass = "WindowClass";
	HINSTANCE hInstance = GetModuleHandle(nullptr);

	SRVEngine.Initialize(hInstance, applicationName, windowClass, 1024, 768);

	std::vector<Vertex3D> ballVertices
	{
		Vertex3D({-1, -1, 1.0f},0, 1),
		Vertex3D({-1, 1, 1.0f},0, 0),
		Vertex3D({1, 1, 1.0f},1, 0),

		Vertex3D({-1, -1, 1.0f},0, 1),
		Vertex3D({1, 1, 1.0f},1, 0),
		Vertex3D({1, -1, 1.0f},1, 1),

		/*Vertex3D({-1, -1, 1.0f}, ),
		Vertex3D({-1, 1, 1.0f}, ),
		Vertex3D({1, 1, 1.0f}, ),
		Vertex3D({1, -1, 1.0f}, ),*/
	};
	std::vector<DWORD> ballIndecies
	{
		0,1,2,
		0,2,3
	};

	GameObject* ball = new GameObject(Vector3D(0.0f, 0.0f, 0.0f));
	ball->AddComponent(new RenderComponent(ball->GetTransform(), ballVertices));
	SRVEngine.AddGameObject(ball);

	GameObject* camera = new GameObject();
	camera->AddComponent(new CameraMovementComponent(SRVEngine.GetGraphics().GetCamera()));
	SRVEngine.AddGameObject(camera);

	while (SRVEngine.ProcessMessages())
	{
		float deltaTime = SRVEngine.GetTimer()->GetMilisecondsElapsed();
		SRVEngine.GetTimer()->Restart();

		SRVEngine.Update(deltaTime);
		SRVEngine.RenderFrame();
	}
}

