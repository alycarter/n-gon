#pragma once
#include "Object.h"

class State;

enum RESOURCE_TYPE { RESOURCE_TYPE_MESH, RESOURCE_TYPE_TEXTURE, RESOURCE_TYPE_SHADER};

class GraphicsResource :
	public Object
{
public:
	GraphicsResource();
	~GraphicsResource();
	RESOURCE_TYPE getType();
	State * getBoundState();
	void bindToState(State * bind);
	bool isCommited();
	void setCommited(bool commit);
protected:
	void setType(RESOURCE_TYPE newType);
private:
	RESOURCE_TYPE type;
	State * boundState;
	bool commited;
};

