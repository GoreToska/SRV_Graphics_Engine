#pragma once

#include "../../../DataTypes/Vector3D.h"
#include "LightComponent.h"

#include <vector>


enum FrustumIntersectionType
{
	Inside,
	Intersects_far_plane,
	Outside,
};

struct AABB
{
	Vector3D min;
	Vector3D max;
};

struct Plane
{
	Plane(const DirectX::SimpleMath::Vector3 normal, float d) : d(d), normal(normal) {};

	Vector3D normal;
	float d;

	float distance(const Vector3D& point) const
	{
		return normal.Dot(point) + d;
	}
};

std::vector<Vector4D> getAABBVerticies(const AABB& box);
std::vector<DWORD> getAABBIndicies();

AABB getAABBForPointLight(const LightComponent& source);

AABB getAABBForSpotLight(const LightComponent& source);

std::vector<Plane> getFrustumPlanes(const std::vector<DirectX::SimpleMath::Vector4>& corners);

Plane createPlane(const Vector4D& a, const Vector4D& b, const Vector4D& c);

FrustumIntersectionType TestAABBFrustum(const AABB& aabb, const std::vector<Plane>& planes);