#pragma once
#include "../../DataTypes/Vector3D.h"


class MouseInputEvent
{
public:
	enum EventType
	{
		Invalid,
		LPress,
		LRelease,
		RPress,
		RRelease,
		MPress,
		MRelease,
		WheelUp,
		WheelDowm,
		Move,
		Delta_Move,
	};

	MouseInputEvent();
	MouseInputEvent(const EventType type, const int x, const int y);

	bool IsValid() const;
	EventType GetType() const;
	Vector2D GetPosition() const;
	int GetPositionX() const;
	int GetPositionY() const;

private:
	EventType type;
	float x;
	float y;
};
