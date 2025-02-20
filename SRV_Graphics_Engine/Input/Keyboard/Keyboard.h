#pragma once

#include <queue>

#include "./KeyboardInputEvent.h"
#include "../../Delegates/Action.h"
#include "../../Delegates/Delegates.h"


class Keyboard
{
public:
	Keyboard();
	Keyboard(Keyboard const&) = delete;
	void operator =(Keyboard const&) = delete;

	static 	Keyboard& GetInstance();

	bool IsKeyPressed(const unsigned char keycode);
	bool KeyBufferIsEmpty();
	bool CharBufferIsEmpty();

	KeyboardInputEvent ReadKey();
	void BroadcastKeyPressed(const unsigned char keycode);
	unsigned char ReadChar();

	void OnKeyPressed(const unsigned char key);
	void OnKeyReleased(const unsigned char key);
	void OnChar(const unsigned char key);

	void EnableAutoRepeatKeys();
	void DisableAutoRepeatKeys();
	void EnableAutoRepeatChars();
	void DisableAutoRepeatChars();
	bool IsKeysAutoRepeat();
	bool IsCharsAutoRepeat();

	// KeyPressedAction.AddRaw(input1, &PongInput::PlayerInput); <- example
	MulticastDelegate<const unsigned char> KeyPressedEvent;
	MulticastDelegate<const unsigned char> KeyReleasedAction;
	MulticastDelegate<const unsigned char> CharPressedAction;

private:
	bool autoRepeatKeys;
	bool autoRepeatChars;
	bool keyStates[256];

	std::queue<KeyboardInputEvent> keyBuffer;
	std::queue<unsigned char> charBuffer;
};
