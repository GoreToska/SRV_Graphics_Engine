#pragma once

#include <DirectXCollision.h>
#include "../Vector3D.h"

class BoundingVolume
{
public:
    virtual ~BoundingVolume() = default;

    virtual bool Intersects(const BoundingVolume& other) const = 0;
    virtual void SetCenter(const Vector3D& position) = 0;
    virtual void SetRadius(const float& radius) = 0;
    virtual float GetRadius() = 0;

    virtual bool IsBox() const = 0;
    virtual bool IsSphere() const = 0;

    virtual const DirectX::BoundingBox& GetBoundingBox() const = 0;
    virtual const DirectX::BoundingSphere& GetBoundingSphere() const = 0;
};