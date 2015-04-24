#pragma once
#include "Object.h"
#include "GraphicsManager.h"
#include <Windows.h>

class StateMachine;
class State;
class Window;
class Controls;
class Time;

class Game :
	public Object
{
public:
	Game();
	~Game();
	void initialize(HINSTANCE hInstance, wchar_t * name, int width, int height);
	void play(State * initialState);
	GraphicsManager * getGraphics();
private:
	void run();
	void update();
	void render();
	void destroy();
	StateMachine * stateMachine;
	Window * window;
	Controls * controls;
	Time * time;
	GraphicsManager graphics;
};

