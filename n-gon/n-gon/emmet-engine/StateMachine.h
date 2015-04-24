#pragma once
#include "Object.h"
#include <Windows.h>
#include <vector>
using namespace std;

class State;
class Time;
class Controls;
class GraphicsManager;
class GraphicsCapture;

struct UpdatePackage;

struct ThreadInfo
{
	vector<GraphicsCapture *> * captures;
	GraphicsManager * manager;
};

enum REQUEST_TYPE{REQUEST_POP, REQUEST_PUSH, REQUEST_CLEARING_PUSH, REQUEST_NONE};

class StateMachine :
	public Object
{
	friend class Game;
public:
	StateMachine();
	~StateMachine();
	void requestPop();
	void requestPush(State * state);
	void requestClearingPush(State * state);
protected:
	bool isEmpty();
	void update(UpdatePackage * package);
	void render(GraphicsManager * graphics);
private:
	void handleChangeRequest(GraphicsManager * graphics);
	vector<State *> states;
	State * requestState;
	REQUEST_TYPE requestType;
	HANDLE renderThread;
	vector<GraphicsCapture *> captures;
	ThreadInfo info;
};

