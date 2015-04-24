#include "Component.h"


Component::Component()
{

}

Component::~Component()
{

}

void Component::requestDelete()
{
	deleteRequested = true;
}

bool Component::isDeleteRequested()
{
	return deleteRequested;
}