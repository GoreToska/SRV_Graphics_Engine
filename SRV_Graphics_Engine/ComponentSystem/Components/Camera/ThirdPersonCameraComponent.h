#pragma once

#include "../IComponent.h"
#include "../../GameObject.h"
#include "../../../Graphics/Camera.h"


class ThirdPersonCameraComponent : public IComponent
{
public:
	ThirdPersonCameraComponent(GameObject* gameObject, GameObject* cameraTarget, Camera* camera);

	void Update(const float& deltaTime) override;

private:

	template <typename T>
	T clamp(T value, T min, T max) {
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}

	GameObject* gameObject;
	GameObject* cameraTarget;
	Camera* camera;

	void UpdateRotation(const float& deltaTime);
	void UpdatePosition(const float& deltaTime);
	void OnMouseMoved(const int x, const int y);

	float mouseSpeed = 0.005;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float distance = 15.0f;

	Vector3D lookDirection;
};