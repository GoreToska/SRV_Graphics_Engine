#include "LightAABB.h"

std::vector<Vector4D> getAABBVerticies(const AABB& box)
{
	std::vector<Vector4D> verticies(8);

	verticies[0] = Vector4D(box.min.x, box.min.y, box.min.z, 0.0f); // 0
	verticies[1] = Vector4D(box.max.x, box.min.y, box.min.z, 0.0f); // 1
	verticies[2] = Vector4D(box.max.x, box.max.y, box.min.z, 0.0f); // 2
	verticies[3] = Vector4D(box.min.x, box.max.y, box.min.z, 0.0f); // 3
	verticies[4] = Vector4D(box.min.x, box.min.y, box.max.z, 0.0f); // 4
	verticies[5] = Vector4D(box.max.x, box.min.y, box.max.z, 0.0f); // 5
	verticies[6] = Vector4D(box.max.x, box.max.y, box.max.z, 0.0f); // 6
	verticies[7] = Vector4D(box.min.x, box.max.y, box.max.z, 0.0f); // 7

	return verticies;
}

std::vector<DWORD> getAABBIndicies()
{
	std::vector<DWORD> indicies = {
		// Front face
		0, 1, 2,
		2, 3, 0,
		// Back face
		5, 4, 7,
		7, 6, 5,
		// Left face
		4, 0, 3,
		3, 7, 4,
		// Right face
		1, 5, 6,
		6, 2, 1,
		// Top face
		3, 2, 6,
		6, 7, 3,
		// Bottom face
		4, 5, 1,
		1, 0, 4
	};

	return indicies;
}

AABB getAABBForPointLight(const LightComponent& source)
{
	Vector3D pos = source.GetLightPosition();
	float radius = source.GetLightDistance();

	AABB box;
	box.min = pos - Vector3D(radius, radius, radius);
	box.max = pos + Vector3D(radius, radius, radius);

	return box;
}

AABB getAABBForSpotLight(const LightComponent& source)
{
	Vector3D pos = source.GetLightPosition();
	Vector3D dir = source.GetLightDirection();
	float length = source.GetLightDistance();
	float radiusAtBase = tanf(source.GetLightAngle()) * length;

	AABB box;
	Vector3D baseCenter = pos + dir * length;

	Vector3D minPoint = Vector3D::Min(pos - Vector3D(radiusAtBase, radiusAtBase, radiusAtBase),
		baseCenter - Vector3D(radiusAtBase, radiusAtBase, radiusAtBase));
	Vector3D maxPoint = Vector3D::Max(pos + Vector3D(radiusAtBase, radiusAtBase, radiusAtBase),
		baseCenter + Vector3D(radiusAtBase, radiusAtBase, radiusAtBase));

	box.min = minPoint;
	box.max = maxPoint;

	return box;
}

std::vector<Plane> getFrustumPlanes(const std::vector<DirectX::SimpleMath::Vector4>& corners)
{
	std::vector<Plane> planes;

	// Near plane
	planes.push_back(createPlane(corners[0], corners[1], corners[2]));
	// Far plane
	planes.push_back(createPlane(corners[5], corners[4], corners[7]));
	// Left
	planes.push_back(createPlane(corners[4], corners[0], corners[5]));
	// Right
	planes.push_back(createPlane(corners[2], corners[6], corners[3]));
	// Top
	planes.push_back(createPlane(corners[1], corners[5], corners[3]));
	// Bottom
	planes.push_back(createPlane(corners[4], corners[0], corners[6]));

	return planes;
}

Plane createPlane(const Vector4D& a, const Vector4D& b, const Vector4D& c)
{
	Vector3D ab = Vector3D(b) - Vector3D(a);
	Vector3D ac = Vector3D(c) - Vector3D(a);
	auto normal = ab.Cross(ac);
	normal.Normalize();
	float d = -normal.Dot(Vector3D(a));

	return Plane(normal, d);
}

FrustumIntersectionType TestAABBFrustum(const AABB& aabb, const std::vector<Plane>& planes)
{
	bool intersectsFar = false;

	for (int i = 0; i < planes.size(); ++i)
	{
		const auto& plane = planes[i];

		DirectX::SimpleMath::Vector3 positiveVertex = aabb.min;
		if (plane.normal.x >= 0) positiveVertex.x = aabb.max.x;
		if (plane.normal.y >= 0) positiveVertex.y = aabb.max.y;
		if (plane.normal.z >= 0) positiveVertex.z = aabb.max.z;

		if (plane.distance(positiveVertex) < 0)
		{
			if (i == 1) // Far plane
				intersectsFar = true;
			else
				return Outside;
		}
	}

	return intersectsFar ? Intersects_far_plane : Inside;
}
