#pragma once

#include "../DataTypes/Vector3D.h"
#include "../ComponentSystem/Components/Light/ShadowMapCalculator.h"

struct VS_ObjectMatrixBuffer
{
	Matrix world;
	Matrix view;
	Matrix projection;
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
	Vector3D dynamicLightColor /*= { 1.0f,0.0f,0.0f }*/;
	float dynamicLightStrength = 1;
	Vector3D dynamicLightDirection = { -0.577f, 0.577f, -0.577f };
};

struct PS_CascadeShadowsBuffer
{
	Matrix ViewProjectionMatrix[ShadowMapCalculator::CascadeCount];
	Vector4D Distances;
	Vector4D CameraPosition;
};