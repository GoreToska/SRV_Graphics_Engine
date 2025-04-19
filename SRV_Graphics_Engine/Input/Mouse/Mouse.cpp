#include "Mouse.h"
#include "MouseInputEvent.h"
#include "../../DataTypes/Vector3D.h"
#include <iostream>


Mouse& Mouse::GetInstance()
{
	static Mouse instance;

	return instance;
}

void Mouse::OnLeftPressed(int x, int y)
{
	leftIsDown = true;
	MouseInputEvent e(MouseInputEvent::EventType::LPress, x, y);
	eventBuffer.push(e);
}

void Mouse::OnLeftReleased(int x, int y)
{
	leftIsDown = false;
	MouseInputEvent e(MouseInputEvent::EventType::LRelease, x, y);
	eventBuffer.push(e);
}

void Mouse::OnRightPressed(int x, int y)
{
	rightIsDown = true;
	MouseInputEvent e(MouseInputEvent::EventType::RPress, x, y);
	eventBuffer.push(e);
}

void Mouse::OnRightReleased(int x, int y)
{
	rightIsDown = false;
	MouseInputEvent e(MouseInputEvent::EventType::RRelease, x, y);
	eventBuffer.push(e);
}

void Mouse::OnMiddlePressed(int x, int y)
{
	middleIsDown = true;
	MouseInputEvent e(MouseInputEvent::EventType::MPress, x, y);
	eventBuffer.push(e);
}

void Mouse::OnMiddleReleased(int x, int y)
{
	middleIsDown = false;
	MouseInputEvent e(MouseInputEvent::EventType::MRelease, x, y);
	eventBuffer.push(e);
}

void Mouse::OnWheelUp(int x, int y)
{
	MouseInputEvent e(MouseInputEvent::EventType::WheelUp, x, y);
	eventBuffer.push(e);
}

void Mouse::OnWheelDown(int x, int y)
{
	MouseInputEvent e(MouseInputEvent::EventType::WheelDowm, x, y);
	eventBuffer.push(e);
}

void Mouse::OnMouseMove(int x, int y)
{
	this->x = x;
	this->y = y;
	MouseInputEvent e(MouseInputEvent::EventType::Move, x, y);
	eventBuffer.push(e);
}

void Mouse::OnMouseDelta(int x, int y)
{
	MouseInputEvent e(MouseInputEvent::EventType::Delta_Move, x, y);
	eventBuffer.push(e);
	OnMouseDeltaEvent.Broadcast(x, y);
}

bool Mouse::IsLeftDown()
{
	return leftIsDown;
}

bool Mouse::IsRightDown()
{
	return rightIsDown;
}

bool Mouse::IsMiddleDown()
{
	return middleIsDown;
}

int Mouse::GetPositionX()
{
	return x;
}

int Mouse::GetPositionY()
{
	return y;
}

Vector2D Mouse::GetPosition()
{
	return { x, y };
}

bool Mouse::EventBufferIsEmpty()
{
	return eventBuffer.empty();
}

MouseInputEvent Mouse::ReadEvent()
{
	if (eventBuffer.empty())
	{
		return MouseInputEvent();
	}
	else
	{
		MouseInputEvent e = eventBuffer.front();
		eventBuffer.pop();
		return e;
	}
}
