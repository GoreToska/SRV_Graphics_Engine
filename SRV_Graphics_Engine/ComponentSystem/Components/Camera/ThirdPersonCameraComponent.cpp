#include "ThirdPersonCameraComponent.h"
#include "../../../Input/Mouse/Mouse.h"


ThirdPersonCameraComponent::ThirdPersonCameraComponent(GameObject* gameObject, GameObject* cameraTarget, Camera* camera)
	: gameObject(gameObject), cameraTarget(cameraTarget), camera(camera)
{
	Mouse::GetInstance().OnMouseDeltaEvent.AddRaw(this, &ThirdPersonCameraComponent::OnMouseMoved);
}

void ThirdPersonCameraComponent::Update(const float& deltaTime)
{
	UpdatePosition(deltaTime);
	UpdateRotation(deltaTime);
	lookDirection = Vector3D::ZeroVector();
}

void ThirdPersonCameraComponent::UpdateRotation(const float& deltaTime)
{
	camera->SetLookAtPosition(cameraTarget->GetTransform()->GetPosition());
}

void ThirdPersonCameraComponent::UpdatePosition(const float& deltaTime)
{
    Vector3D targetPosition = cameraTarget->GetTransform()->GetPosition();

    float horizontalDistance = distance * cos(pitch);
    float verticalDistance = distance * sin(pitch);

    float cameraX = targetPosition.x + horizontalDistance * cos(yaw);
    float cameraY = targetPosition.y + verticalDistance;
    float cameraZ = targetPosition.z + horizontalDistance * sin(yaw);

    camera->SetPosition(Vector3D(cameraX, cameraY, cameraZ));
}

void ThirdPersonCameraComponent::OnMouseMoved(const int x, const int y)
{
    yaw += -x * mouseSpeed;
    pitch += y * mouseSpeed;

    pitch = clamp(pitch, -DirectX::XM_PIDIV2 + 0.1f, DirectX::XM_PIDIV2 - 0.1f);
}
