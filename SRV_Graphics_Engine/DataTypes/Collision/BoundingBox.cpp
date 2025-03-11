#include "BoundingBox.h"

BoundingBox::BoundingBox(const DirectX::BoundingBox& box) :
	boundingBox(box)
{

}

bool BoundingBox::Intersects(const BoundingVolume& other) const
{
	if (other.IsBox())
	{
		return boundingBox.Intersects(other.GetBoundingBox());
	}

	if (other.IsSphere())
	{
		return boundingBox.Intersects(other.GetBoundingSphere());
	}

	return false;
}

void BoundingBox::SetCenter(const Vector3D& position)
{
	boundingBox.Center = { position.x, position.y, position.z };
}

const DirectX::BoundingBox& BoundingBox::GetBoundingBox() const
{
	return boundingBox;
}

const DirectX::BoundingSphere& BoundingBox::GetBoundingSphere() const
{
	static DirectX::BoundingSphere emptySphere;
	return emptySphere;
}
