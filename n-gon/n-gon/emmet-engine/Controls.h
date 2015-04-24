#pragma once
#include "Object.h"
#include <Windows.h>
#include <vector>
using namespace std;

class Controls :
	public Object
{
public:
	Controls(void); // defualt constructor
	~Controls(void); // defualt destructor
	void handleKeyPressed(WPARAM keyCode); //adds that keycode to the keys pressed list
	void handleKeyRelease(WPARAM keyCode); // removes that keycode form the keys pressed and adds it to the key released
	bool isKeyReleased(WPARAM keyCode); // returns true is the key is in the keys released list
	bool isKeyPressed(WPARAM keyCode); // returns true if the key is being pressed
	void clearKeysReleased(); // called at the end of the game update. cleares all keys that have been released during the previous frame
private:
	vector<WPARAM> keysPressed; // holds the keys that are held down
	vector<WPARAM> keysReleased; // holds the keys released during the prior frame
};