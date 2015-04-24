#include "GraphicsResource.h"

#define NULL 0

GraphicsResource::GraphicsResource()
{
	boundState = NULL;
}

GraphicsResource::~GraphicsResource()
{

}

RESOURCE_TYPE GraphicsResource::getType()
{
	return type;
}

State * GraphicsResource::getBoundState()
{
	return boundState;
}

void GraphicsResource::setType(RESOURCE_TYPE newType)
{
	type = newType;
}

void GraphicsResource::bindToState(State * bind)
{
	boundState = bind;
}

bool GraphicsResource::isCommited()
{
	return commited;
}

void GraphicsResource::setCommited(bool commit)
{
	commited = commit;
}