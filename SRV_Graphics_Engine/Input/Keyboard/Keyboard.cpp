#include "Keyboard.h"

Keyboard::Keyboard()
{
	autoRepeatKeys = false;
	autoRepeatChars = false;

	for (int i = 0; i < 256; ++i)
	{
		keyStates[i] = false;
	}
}

Keyboard& Keyboard::GetInstance()
{
	static Keyboard instance;

	return instance;
}

bool Keyboard::IsKeyPressed(const unsigned char keycode)
{
	return keyStates[keycode];
}

bool Keyboard::KeyBufferIsEmpty()
{
	return keyBuffer.empty();
}

bool Keyboard::CharBufferIsEmpty()
{
	return charBuffer.empty();
}

KeyboardInputEvent Keyboard::ReadKey()
{
	if (KeyBufferIsEmpty())
	{
		return KeyboardInputEvent();
	}
	else
	{
		KeyboardInputEvent e = keyBuffer.front();
		keyBuffer.pop();

		return e;
	}
}

unsigned char Keyboard::ReadChar()
{
	if (charBuffer.empty())
	{
		return 0u;
	}
	else
	{
		unsigned char e = charBuffer.front();
		charBuffer.pop();

		return e;
	}
}

void Keyboard::OnKeyPressed(const unsigned char key)
{
	keyStates[key] = true;
	keyBuffer.push(KeyboardInputEvent(KeyboardInputEvent::EventType::Press, key));
}

void Keyboard::OnKeyReleased(const unsigned char key)
{
	keyStates[key] = false;
	keyBuffer.push(KeyboardInputEvent(KeyboardInputEvent::EventType::Release, key));
}

void Keyboard::OnChar(const unsigned char key)
{
	charBuffer.push(key);
}

void Keyboard::EnableAutoRepeatKeys()
{
	autoRepeatKeys = true;
}

void Keyboard::DisableAutoRepeatKeys()
{
	autoRepeatKeys = false;
}

void Keyboard::EnableAutoRepeatChars()
{
	autoRepeatChars = true;
}

void Keyboard::DisableAutoRepeatChars()
{
	autoRepeatChars = false;
}

bool Keyboard::IsKeysAutoRepeat()
{
	return autoRepeatKeys;
}

bool Keyboard::IsCharsAutoRepeat()
{
	return autoRepeatChars;
}
