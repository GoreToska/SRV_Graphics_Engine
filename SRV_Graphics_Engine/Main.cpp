#pragma region Includes

#include <Windows.h>
#include <iostream>
#include <tuple>
#include <filesystem>

#include "./Utils/Logger.h"
#include "./Engine/Engine.h"

#include "Graphics/ImGui/imgui.h"
#include "Graphics/ImGui/imgui_impl_dx11.h"
#include "Graphics/ImGui/imgui_impl_win32.h"

#include "./Input/Keyboard/Keyboard.h"
#include"./Shapes/Shapes.h"
#include "ComponentSystem/GameObject.h"
#include "ComponentSystem/Components/Collision/SphereCollisionComponent.h"  
#include "Games/Katamari/KatamariMovementComponent.h"
#include "Games/Katamari/KatamariCollisionComponent.h"
#include "ComponentSystem/Components/Camera/TopDownCameraComponent.h"
#include "ComponentSystem/Components/Camera/CameraMovementComponent.h"
#include "ComponentSystem/Components/Camera/ThirdPersonCameraComponent.h"
#include "DataTypes/ModelData.h"
#include "./ComponentSystem/Components/Render/PrimitiveRenderComponent.h"
#include "ComponentSystem/Components/Light/LightComponent.h"
#include "ComponentSystem/Components/Particles/ExplosionParticleSystem.h"



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
	ModelData groundModelData = { "Data\\Models\\Ground\\Ground.obj", L"Data\\Models\\Ground\\Ground.jpg" };

	const int gridSize = 500;
	const float squareSize = 50.0f;
	const float h = 0.0f;

	std::vector<TVertex> groundVertexes;
	std::vector<DWORD> groundIndexes;

	for (int i = 0; i <= gridSize; ++i)
	{
		for (int j = 0; j <= gridSize; ++j)
		{
			float x = squareSize / 2 + (squareSize / gridSize) * i;
			float z = squareSize / 2 + (squareSize / gridSize) * j;
			float u = x / 100;
			float v = z / 100;
			groundVertexes.emplace_back(TVertex({ x, h, z }, u, v, Vector3D(0, 1, 0)));
		}
	}

	for (int i = 0; i < gridSize; ++i)
	{
		for (int j = 0; j < gridSize; ++j)
		{
			DWORD topLeft = i * (gridSize + 1) + j;
			DWORD topRight = topLeft + 1;
			DWORD bottomLeft = (i + 1) * (gridSize + 1) + j;
			DWORD bottomRight = bottomLeft + 1;

			groundIndexes.push_back(topLeft);
			groundIndexes.push_back(topRight);
			groundIndexes.push_back(bottomLeft);

			groundIndexes.push_back(topRight);
			groundIndexes.push_back(bottomRight);
			groundIndexes.push_back(bottomLeft);
		}
	}

	static float directionalLightPosition[3] = { 0,40,-30 };
	static float directionalLightRotation[3] = { 0,0,0 };

	static float pointLightPosition[3] = { 2,2,2 };

	static float spotLightPosition[3] = { 0,5,0 };
	static float spotLightRotation[3] = { 0,1.4f,0 };


	GameObject* directionalLight = new GameObject(Vector3D(
		directionalLightPosition[0], directionalLightPosition[1], directionalLightPosition[2]));
	directionalLight->GetTransform()->SetRotation({
		directionalLightRotation[0], directionalLightRotation[1],directionalLightRotation[2]});
	directionalLight->AddComponent(new LightComponent(directionalLight, Directional));
	directionalLight->GetTransform()->SetScale({ 0.01,0.01,0.01 });
	SRVEngine.AddGameObject(directionalLight);
	directionalLight->GetComponent<LightComponent>()->SetLightColor({ 1, 1, 1 });
	directionalLight->GetComponent<LightComponent>()->SetLightStrength(3);

	
	GameObject* pointLight = new GameObject({ pointLightPosition[0], pointLightPosition[1], pointLightPosition[2] });
	pointLight->AddComponent(new LightComponent(pointLight, Point));
	SRVEngine.AddGameObject(pointLight);
	pointLight->GetComponent<LightComponent>()->SetLightColor({ 0, 1, 0 });
	pointLight->GetComponent<LightComponent>()->SetLightStrength(1);

	GameObject* spotLight = new GameObject({ spotLightPosition[0], spotLightPosition[1], spotLightPosition[2] });
	spotLight->AddComponent(new LightComponent(spotLight, Spot));
	SRVEngine.AddGameObject(spotLight);
	spotLight->GetComponent<LightComponent>()->SetLightColor({ 1, 0, 0 });
	spotLight->GetComponent<LightComponent>()->SetLightStrength(2);

	GameObject* blueBird = new GameObject(Vector3D(15.0f, 0.0f, -15.0f));
	auto t = new MeshRendererComponent(blueBirdModelData, blueBird, ShaderManager::ShaderType::Texture);
	//t->SetDecal(decal);
	blueBird->AddComponent(t);
	
	blueBird->AddComponent(new SphereCollisionComponent(blueBird, Vector3D(0.0f, 0.0f, 0.0f), 2.4));
	blueBird->GetTransform()->SetScale(Vector3D(0.02, 0.02, 0.02));
	SRVEngine.AddGameObject(blueBird);
	

	/*GameObject* phone = new GameObject(Vector3D(-25.0f, 0.0f, 5.0f));
	phone->AddComponent(new MeshRendererComponent(phoneModelData, phone, ShaderManager::ShaderType::Texture));
	phone->AddComponent(new SphereCollisionComponent(phone, Vector3D(0.0f, 0.0f, 0.0f), 3));
	phone->GetTransform()->SetScale(Vector3D(0.04, 0.04, 0.04));
	SRVEngine.AddGameObject(phone);*/

	/*GameObject* hat = new GameObject(Vector3D(-25.0f, 1.0f, 20.0f));
	hat->AddComponent(new MeshRendererComponent(hatModelData, hat, ShaderManager::ShaderType::Texture));
	hat->AddComponent(new SphereCollisionComponent(hat, Vector3D(0.0f, 0.0f, 0.0f), 2));
	hat->GetTransform()->SetScale(Vector3D(0.2, 0.2, 0.2));
	SRVEngine.AddGameObject(hat);*/

	/*GameObject* memoryCard = new GameObject(Vector3D(-20.0f, 0.0f, -20.0f));
	memoryCard->AddComponent(new MeshRendererComponent(memoryCardModelData, memoryCard, ShaderManager::ShaderType::Texture));
	memoryCard->AddComponent(new SphereCollisionComponent(memoryCard, Vector3D(0.0f, 0.0f, 0.0f), 3));
	memoryCard->GetTransform()->SetScale(Vector3D(1, 1, 1));
	SRVEngine.AddGameObject(memoryCard);*/

	/*GameObject* radio = new GameObject(Vector3D(-20.0f, 0.0f, 10.0f));
	radio->AddComponent(new MeshRendererComponent(radioModelData, radio, ShaderManager::ShaderType::Texture));
	radio->AddComponent(new SphereCollisionComponent(radio, Vector3D(0.0f, 0.0f, 0.0f), 2.7));
	radio->GetTransform()->SetScale(Vector3D(1, 1, 1));
	SRVEngine.AddGameObject(radio);*/

	/*GameObject* kettle = new GameObject(Vector3D(-15.0f, 0.0f, 10.0f));
	kettle->AddComponent(new MeshRendererComponent(kettleModelData, kettle, ShaderManager::ShaderType::Texture));
	kettle->AddComponent(new SphereCollisionComponent(kettle, Vector3D(0.0f, 0.0f, 0.0f), 3));
	kettle->GetTransform()->SetScale(Vector3D(0.2, 0.2, 0.2));
	SRVEngine.AddGameObject(kettle);*/

	GameObject* octopus = new GameObject(Vector3D(10.0f, 0.0f, -10.0f));
	t = new MeshRendererComponent(octopusModelData, octopus, ShaderManager::ShaderType::Texture);
	//t->SetDecal(decal);
	octopus->AddComponent(t);
	octopus->AddComponent(new SphereCollisionComponent(octopus, Vector3D(0.0f, 0.0f, 0.0f), 4));
	octopus->GetTransform()->SetScale(Vector3D(3, 3, 3));
	SRVEngine.AddGameObject(octopus);

	GameObject* amongus = new GameObject(Vector3D(5.0f, 0.0f, 0.0f));
	t = new MeshRendererComponent(amongusModelData, amongus, ShaderManager::ShaderType::Texture);
	//t->SetDecal(decal);
	amongus->AddComponent(t);
	amongus->AddComponent(new SphereCollisionComponent(amongus, Vector3D(0.0f, 0.0f, 0.0f), 2));
	amongus->GetTransform()->SetScale(Vector3D(0.03, 0.03, 0.03));
	SRVEngine.AddGameObject(amongus);

	GameObject* greenPig = new GameObject(Vector3D(10.0f, 0.0f, 10.0f));
	t = new MeshRendererComponent(greenPigModelData, greenPig, ShaderManager::ShaderType::Texture);
	//t->SetDecal(decal);
	greenPig->AddComponent(t);
	greenPig->AddComponent(new SphereCollisionComponent(greenPig, Vector3D(0.0f, 0.0f, 0.0f), 2));
	greenPig->GetTransform()->SetScale(Vector3D(0.02, 0.02, 0.02));
	SRVEngine.AddGameObject(greenPig);

	/*GameObject* redBird = new GameObject(Vector3D(0.0f, 0.0f, 0.0f));
	redBird->AddComponent(new MeshRendererComponent(redBirdModelData, redBird, ShaderManager::ShaderType::Texture));
	redBird->AddComponent(new SphereCollisionComponent(redBird, Vector3D(0, 0, 0), 2.2));
	redBird->AddComponent(new KatamariMovementComponent(redBird));
	redBird->GetTransform()->SetScale(Vector3D(0.01, 0.01, 0.01));
	redBird->AddComponent(new KatamariCollisionComponent(redBird));
	SRVEngine.AddGameObject(redBird);*/

	GameObject* camera = new GameObject(Vector3D(15, 15, 15));
	camera->AddComponent(new CameraMovementComponent(SRVEngine.GetGraphics().GetCamera()));
	SRVEngine.GetGraphics().GetCamera()->SetPosition(Vector3D{0,3,0});
	//camera->AddComponent(new TopDownCameraComponent(SRVEngine.GetGraphics().GetCamera(), redBird, Vector3D(0, 20, -10)));
	//camera->AddComponent(new ThirdPersonCameraComponent(camera, redBird, SRVEngine.GetGraphics().GetCamera()));
	SRVEngine.AddGameObject(camera);

	GameObject* ground = new GameObject(Vector3D(0.0f, -3.0f, 0.0f));
	ground->GetTransform()->SetScale({ 10,10,10 });
	t = new MeshRendererComponent(groundModelData, ground, ShaderManager::ShaderType::Texture);
	//t->SetDecal(decal);
	ground->AddComponent(t);
	SRVEngine.AddGameObject(ground);

	GameObject* particles = new GameObject(Vector3D(0.0f, 2.0f, 0.0f));
	auto sys = new ExplosionParticleSystem(particles, 512);
	particles->AddComponent(sys);
	sys->Initialize();
	SRVEngine.AddGameObject(particles);


	while (SRVEngine.ProcessMessages())
	{
		float deltaTime = SRVEngine.GetTimer()->GetMilisecondsElapsed();
		SRVEngine.GetTimer()->Restart();

		//greenPig->GetTransform()->AddLocalRotation({ 0.01,0.01,0.01 });

		directionalLight->GetTransform()->SetRotation({directionalLightRotation[0], directionalLightRotation[1],directionalLightRotation[2] });
		directionalLight->GetTransform()->SetPosition({directionalLightPosition[0], directionalLightPosition[1], directionalLightPosition[2] });
		pointLight->GetTransform()->SetPosition({ pointLightPosition[0], pointLightPosition[1], pointLightPosition[2]});
		spotLight->GetTransform()->SetPosition({spotLightPosition[0],spotLightPosition[1], spotLightPosition[2]});
		spotLight->GetTransform()->SetRotation({spotLightRotation[0],spotLightRotation[1], spotLightRotation[2]});

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Light Settings");
		
		ImGui::DragFloat3("Directional Light Position", directionalLightPosition, 0.05, -100, 100);
		ImGui::DragFloat3("Directional Light Rotation", directionalLightRotation, 0.05, -100, 100);
		ImGui::Text("-------------");
		ImGui::DragFloat3("Point Light Position", pointLightPosition, 0.05, -100, 100);
		ImGui::Text("-------------");
		ImGui::DragFloat3("Spot Light Position", spotLightPosition, 0.05, -100, 100);
		ImGui::DragFloat3("Spot Light Rotation", spotLightRotation, 0.05, -100, 100);

		ImGui::End();


		SRVEngine.Update(deltaTime);
		SRVEngine.RenderFrame();
	}
}

