#pragma once

#include "BoundingVolume.h"


class BoundingSphere : public BoundingVolume
{
public:
    BoundingSphere(const DirectX::BoundingSphere& sphere);

    bool Intersects(const BoundingVolume& other) const override;
    void SetCenter(const Vector3D& position) override;

    bool IsBox() const override { return false; }
    bool IsSphere() const override { return true; }

    const DirectX::BoundingBox& GetBoundingBox() const override;
    const DirectX::BoundingSphere& GetBoundingSphere() const override;

private:
    DirectX::BoundingSphere boundingSphere;
};

