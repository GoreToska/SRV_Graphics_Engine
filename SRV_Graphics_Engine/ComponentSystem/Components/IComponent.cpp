#include "IComponent.h"

bool IComponent::IsEnabled()
{
	return isEnabled;
}

void IComponent::SetEnabled(bool value)
{
	isEnabled = value;
}
