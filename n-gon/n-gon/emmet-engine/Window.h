#pragma once
#include "Object.h"
#include <Windows.h>

class Controls;

class Window :
	public Object
{
public:
	Window(void);
	~Window(void);
	void create(wchar_t * name, int x, int y, int width, int height, HINSTANCE hInstance);
	void show();
	void hide();
	void readMessages(Controls * controls);
	bool isQuitRequested();
	int getWidth();
	int getHeight();
	HWND getWindowHandle();
private:
	int width;
	int height;
	HWND handle;
	bool quitRequested;
};

