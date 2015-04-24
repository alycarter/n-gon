#include "StateMachine.h"
#include "State.h"
#include "UpdatePackage.h"
#include "GraphicsRenderer.h"
#include "GraphicsManager.h"

DWORD WINAPI draw(LPVOID ptr);

DWORD WINAPI draw(LPVOID ptr)
{
	ThreadInfo * info = (ThreadInfo *)ptr;
	GraphicsRenderer renderer;
	renderer.drawGraphicsCaptures(info->captures, info->manager);
	return 0;
}

StateMachine::StateMachine()
{
	
}

StateMachine::~StateMachine()
{
	WaitForSingleObject(renderThread, INFINITE);
	while (!states.empty())
	{
		delete states.at(states.size() - 1);
		states.pop_back();
	}
}

bool StateMachine::isEmpty()
{
	return states.empty();
}

void StateMachine::update(UpdatePackage * package)
{
	package->stateMachine = this;
	//swap any changes to the active state
	handleChangeRequest(package->graphics);
	if (!states.empty())
	{
		states.at(states.size()-1)->update(package);
	}
}

void StateMachine::render(GraphicsManager * graphics)
{
	//capture all states
	for (int i = states.size() - 1; i >= 0; i--)
	{
		states.at(i)->render(graphics);
		if (states.at(i)->isOccluding())
		{
			i = -i;
		}
	}
	//wait for last render to end
	WaitForSingleObject(renderThread, INFINITE);
	//commit any loaded assets
	graphics->commitNewResources();
	captures.clear();
	for (int i = states.size() - 1; i >= 0; i--)
	{
		//get the capture
		captures.push_back(states.at(i)->getFrontCaptureBuffer());
		//swap the buffers of the active state
		states.at(i)->swapCaptureBuffers();
		if (states.at(i)->isOccluding())
		{
			i = -i;
		}
	}
	info.captures = &captures;
	info.manager = graphics;
	renderThread = CreateThread(NULL, 0, draw, &info, 0, NULL);
}

void StateMachine::requestPop()
{
	requestType = REQUEST_POP;
	if (requestState != NULL)
	{
		printf("warning state push requested before pop \n");
		delete requestState;
	}
	requestState = NULL;
}

void StateMachine::requestPush(State * state)
{
	requestType = REQUEST_PUSH;
	if (requestState != NULL)
	{
		delete requestState;
		printf("warning state push already requested before push \n");
	}
	requestState = state;
}

void StateMachine::requestClearingPush(State * state)
{
	requestType = REQUEST_CLEARING_PUSH;
	if (requestState != NULL)
	{
		delete requestState;
		printf("warning state push already requested before clearing push \n");
	}
	requestState = state;
}

void StateMachine::handleChangeRequest(GraphicsManager * graphics)
{
	switch (requestType)
	{
	case REQUEST_CLEARING_PUSH:
		WaitForSingleObject(renderThread, INFINITE);
		while(!states.empty())
		{
			graphics->clearStateBoundResources(states.at(states.size() - 1));
			delete states.at(states.size() - 1);
			states.pop_back();
		}
		states.push_back(requestState);
		break;
	case REQUEST_PUSH:
		states.push_back(requestState);
		break;
	case REQUEST_POP:
		WaitForSingleObject(renderThread, INFINITE);
		if (!states.empty())
		{
			graphics->clearStateBoundResources(states.at(states.size() - 1));
			delete states.at(states.size() - 1);
			states.pop_back();
		}
		break;
	default:
		break;
	}
	requestType = REQUEST_NONE;
	requestState = NULL;
}