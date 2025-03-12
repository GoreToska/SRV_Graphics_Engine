#pragma once

#include "BoundingVolume.h"

class BoundingBox : public BoundingVolume
{
public:
	BoundingBox(const DirectX::BoundingBox& box);

	bool Intersects(const BoundingVolume& other) const override;
	void SetCenter(const Vector3D& position) override;
	void SetRadius(const float& radius) override;
	float GetRadius() override;

	bool IsBox() const override { return true; }
	bool IsSphere() const override { return false; }

	const DirectX::BoundingBox& GetBoundingBox() const override;
	const DirectX::BoundingSphere& GetBoundingSphere() const override;

private:
	DirectX::BoundingBox boundingBox;
};

