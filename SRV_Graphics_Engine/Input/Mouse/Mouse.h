#pragma once
#include <queue>

#include "MouseInputEvent.h"
#include "../../Delegates/Delegates.h"


class Mouse
{
public:
	static Mouse& GetInstance();

	void OnLeftPressed(int x, int y);
	void OnLeftReleased(int x, int y);
	void OnRightPressed(int x, int y);
	void OnRightReleased(int x, int y);
	void OnMiddlePressed(int x, int y);
	void OnMiddleReleased(int x, int y);
	void OnWheelUp(int x, int y);
	void OnWheelDown(int x, int y);
	void OnMouseMove(int x, int y);
	void OnMouseDelta(int x, int y);

	bool IsLeftDown();
	bool IsRightDown();
	bool IsMiddleDown();

	int GetPositionX();
	int GetPositionY();
	Vector2D GetPosition();

	bool EventBufferIsEmpty();
	MouseInputEvent ReadEvent();

	MulticastDelegate<const int, const int> OnMouseDeltaEvent;

private:
	std::queue<MouseInputEvent> eventBuffer;

	bool leftIsDown = false;
	bool rightIsDown = false;
	bool middleIsDown = false;

	float x = 0;
	float y = 0;
};

