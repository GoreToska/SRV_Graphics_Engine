#include "MouseInputEvent.h"
#include "../../DataTypes/VectorInt2D.h"

MouseInputEvent::MouseInputEvent() : type(EventType::Invalid), x(0), y(0) {}

MouseInputEvent::MouseInputEvent(const EventType type, const int x, const int y) : type(type), x(x), y(y) {}

bool MouseInputEvent::IsValid() const
{
	return type != EventType::Invalid;
}

MouseInputEvent::EventType MouseInputEvent::GetType() const
{
	return type;
}

VectorInt2D MouseInputEvent::GetPosition() const
{
	return { x, y };
}

int MouseInputEvent::GetPositionX() const
{
	return x;
}

int MouseInputEvent::GetPositionY() const
{
	return y;
}



