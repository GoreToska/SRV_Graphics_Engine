#pragma once

#include "../DataTypes/Vector3D.h"
#include "../ComponentSystem/Components/Light/ShadowMapCalculator.h"

enum LightSourceType
{
	Directional = 1,
	Spot = 2,
	Point = 3,
};

struct VS_ObjectMatrixBuffer
{
	Matrix world;
	Matrix view;
	Matrix projection;
	Matrix inverseView;
	Matrix inverseProjection;
};

struct VS_LightMatrixBuffer
{
	Matrix lightWorld;
	Matrix lightView;
	Matrix lightProjection;
};

struct PS_LightParamsBuffer
{
	Vector3D ambientLightColor = { 1.0f,1.0f,1.0f };
	float ambientLightStrength = 0.1;
	Vector3D lightColor = { 1.0f,0.0f,0.0f };
	float lightStrength = 1;
	Vector4D lightDirection = { -0.577f, 0.577f, -0.577f, 0 };
	Vector3D lightPosition;
	int sourceType;
	float angle;
};

struct PS_CascadeShadowsBuffer
{
	Matrix ViewProjectionMatrix[ShadowMapCalculator::CascadeCount];
	Vector4D Distances;
	Vector4D CameraPosition;
};

struct CS_Sort
{
	UINT iLevel;
	UINT iLevelMask;
	UINT iWidth;
	UINT iHeight;
};