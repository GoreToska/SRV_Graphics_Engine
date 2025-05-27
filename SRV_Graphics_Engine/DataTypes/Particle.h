#pragma once
#include "Vector3D.h"
#include "../ComponentSystem/GameObject.h"

struct SortListStruct
{
	SortListStruct(unsigned int index, float squaredDistance) : index(index), squaredDistance(squaredDistance) {};
	SortListStruct() {};
	unsigned int index;
	float squaredDistance;
};

struct Particle
{
	Vector4D initialColor = Vector4D{1,1,1,1};
	Vector4D endColor = Vector4D{ 0,1,0,1 };
	Vector3D position;								float _1;
	Vector3D prevPosition;							float _2;
	Vector3D velocity;								float _3;
	Vector3D acceleration = Vector3D{ 0,0.5,0 };	float _4;
	float maxLifetime = 100;
	float lifetime;
	float initialSize = 1.0f;
	float endSize = 2.0f;
	float initialWeight = 1.0f;
	float endWeight = 1.0f;
};