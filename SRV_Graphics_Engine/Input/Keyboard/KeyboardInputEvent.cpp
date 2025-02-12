#include "KeyboardInputEvent.h"

KeyboardInputEvent::KeyboardInputEvent() : type(EventType::Invalid), key(0u) {}

KeyboardInputEvent::KeyboardInputEvent(const EventType type, const unsigned char key) : type(type), key(key) {}

bool KeyboardInputEvent::IsPress() const
{
	return type == EventType::Press;
}

bool KeyboardInputEvent::IsRelease() const
{
	return type == EventType::Release;
}

bool KeyboardInputEvent::IsValid() const
{
	return type != EventType::Invalid;
}

unsigned char KeyboardInputEvent::GetKeyCode() const
{
	return key;
}

