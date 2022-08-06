#pragma once
#include <Windows.h>
//git
bool getkey(char Key);

class Keyboard {
public:
	enum EventType
	{
		Press,
		Release,
		Invalid
	};
	Keyboard();
	//checks if its down
	bool isKeyPressed(unsigned char letter);
	//checks if its Released
	bool isKeyReleased(unsigned char letter);


	//set keyReleased
	void onKeyReleased(const unsigned char letter);
	//set keyDown
	void onKeyDown(const unsigned char letter);

private:
	bool KeysDown[256];
	bool oKeyReleased[256];//only checks this one once
	EventType type;

};
