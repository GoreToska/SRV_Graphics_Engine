#pragma once

#include "../../ComponentSystem/Components/IComponent.h"
#include "../../ComponentSystem/Components/CollisionComponent.h"
#include "../../DataTypes/Vertex.h"
#include<d3d11.h>

class PongSpawnerComponent : public IComponent
{
public:
	PongSpawnerComponent(GameObject* ownerBall);

	void Update(float deltaTime) override;

	void OnCollision(CollisionComponent* owner, CollisionComponent* other);
	void SpawnBalls(int i);
private:
	int ballSpawnCount = 1;
	GameObject* owner;

	std::vector<Vertex3D> ballVertices
	{
		Vertex3D({-0.01f, -0.02f, 1.0f}, {1.0f, 1.0f, 1.0f}),
		Vertex3D({-0.01f, 0.02f, 1.0f}, {1.0f, 1.0f, 1.0f}),
		Vertex3D({0.01f, 0.02f, 1.0f}, {1.0f, 1.0f, 1.0f}),
		Vertex3D({0.01f, -0.02f, 1.0f}, {1.0f, 1.0f, 1.0f}),
	};
	std::vector<DWORD> ballIndecies
	{
		0,1,2,
		0,2,3
	};
};

