#pragma once
#include "Vector3D.h"
#include "../ComponentSystem/GameObject.h"

struct SortListStruct
{
	SortListStruct(size_t index, float squaredDistance) : index(index), squaredDistance(squaredDistance) {};
	SortListStruct() {};
	size_t index;
	float squaredDistance;
};

struct Particle
{
	Vector4D initialColor;
	Vector4D endColor;
	Vector3D position;		float _1;
	Vector3D prevPosition;	float _2;
	Vector3D velocity;		float _3;
	Vector3D acceleration;	float _4;
	float maxLifetime;
	float lifetime;
	float initialSize = 1.0f;
	float endSize = 1.0f;
	float initialWeight = 1.0f;
	float endWeight = 1.0f;
};