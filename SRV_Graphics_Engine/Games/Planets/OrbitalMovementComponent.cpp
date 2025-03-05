#include "OrbitalMovementComponent.h"


OrbitalMovementComponent::OrbitalMovementComponent(GameObject* owner,
	GameObject* orbit, Vector3D axis, float speed, float radius) :
	owner(owner), orbit(orbit), speed(speed), radius(radius), theta(0), axis(axis)
{
}

void OrbitalMovementComponent::Update(const float& deltaTime)
{
	using namespace DirectX;

    // Увеличиваем текущий угол на основе скорости вращения и времени
    theta += speed * deltaTime;

    // Ограничиваем угол в пределах [0, 2π]
    if (theta > XM_2PI)
    {
        theta -= XM_2PI;
    }

    // Нормализуем ось вращения
    XMVECTOR axis = XMVectorSet(this->axis.x, this->axis.y, this->axis.z, 0.0f);
    axis = XMVector3Normalize(axis);

    // Выбираем вспомогательный вектор "вверх" в зависимости от оси вращения
    XMVECTOR upVector;
    if (fabs(XMVectorGetX(axis)) < 0.1f && fabs(XMVectorGetZ(axis)) < 0.1f)
    {
        // Если ось вращения близка к оси Y, используем ось Z как "вверх"
        upVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    }
    else
    {
        // Иначе используем ось Y как "вверх"
        upVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    }

    // Вычисляем перпендикулярные векторы для плоскости вращения
    XMVECTOR perpendicularVector = XMVector3Cross(upVector, axis); // Перпендикуляр к оси и "вверх"
    perpendicularVector = XMVector3Normalize(perpendicularVector);

    // Вычисляем второй перпендикулярный вектор
    XMVECTOR binormalVector = XMVector3Cross(axis, perpendicularVector);
    binormalVector = XMVector3Normalize(binormalVector);

    // Вычисляем новое положение объекта на орбите
    float cosAngle = cosf(theta);
    float sinAngle = sinf(theta);

    XMVECTOR orbitPosition = XMVectorSet(orbit->GetTransform()->GetPosition().x,
        orbit->GetTransform()->GetPosition().y,
        orbit->GetTransform()->GetPosition().z, 0.0f) +
        radius * (cosAngle * perpendicularVector + sinAngle * binormalVector);

    // Обновляем текущее положение объекта
    owner->GetTransform()->SetPosition(Vector3D(XMVectorGetX(orbitPosition),
        XMVectorGetY(orbitPosition),
        XMVectorGetZ(orbitPosition)));
}
