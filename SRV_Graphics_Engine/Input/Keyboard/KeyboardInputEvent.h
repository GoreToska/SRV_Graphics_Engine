#pragma once


class KeyboardInputEvent
{
public:
	enum EventType
	{
		Invalid,
		Press,
		Release,
	};

	KeyboardInputEvent();
	KeyboardInputEvent(const EventType type, const unsigned char key);

	bool IsPress() const;
	bool IsRelease() const;
	bool IsValid() const;
	unsigned char GetKeyCode() const;

private:
	EventType type;
	unsigned char key;
};

