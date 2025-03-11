#include "TopDownCameraComponent.h"


TopDownCameraComponent::TopDownCameraComponent(Camera* camera, GameObject* followObject, Vector3D offset) :
	camera(camera), followObject(followObject), offset(offset)
{
}

void TopDownCameraComponent::Update(const float& deltaTime)
{
	if (!IsEnabled())
		return;

	UpdatePosition();
}

void TopDownCameraComponent::UpdatePosition()
{
	camera->SetPosition(followObject->GetTransform()->GetPosition());
	camera->AddPosition(offset);
	camera->SetLookAtPosition(followObject->GetTransform()->GetPosition());
}
