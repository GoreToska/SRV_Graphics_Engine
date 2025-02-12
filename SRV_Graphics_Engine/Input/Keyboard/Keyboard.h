#pragma once

#include <queue>

#include "./KeyboardInputEvent.h"

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

private:
	bool autoRepeatKeys;
	bool autoRepeatChars;
	bool keyStates[256];

	std::queue<KeyboardInputEvent> keyBuffer;
	std::queue<unsigned char> charBuffer;
};
