#include "BoundingSphere.h"

BoundingSphere::BoundingSphere(const DirectX::BoundingSphere& sphere) :
	boundingSphere(sphere)
{

}

bool BoundingSphere::Intersects(const BoundingVolume& other) const
{
	if (other.IsBox())
	{
		return boundingSphere.Intersects(other.GetBoundingBox());

	}

	if (other.IsSphere())
	{
		return boundingSphere.Intersects(other.GetBoundingSphere());
	}

	return false;
}

void BoundingSphere::SetCenter(const Vector3D& position)
{
	boundingSphere.Center = { position.x, position.y, position.z };
}

void BoundingSphere::SetRadius(const float& radius)
{
	boundingSphere.Radius = radius;
}

float BoundingSphere::GetRadius()
{
	return boundingSphere.Radius;
}

const DirectX::BoundingBox& BoundingSphere::GetBoundingBox() const
{
	static DirectX::BoundingBox emptyBox;
	return emptyBox;
}

const DirectX::BoundingSphere& BoundingSphere::GetBoundingSphere() const
{
	return boundingSphere;
}
