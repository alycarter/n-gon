#include "Entity.h"
#include "Component.h"
#include "UpdatePackage.h"
#include "Transform.h"
#include "RenderPass.h"

Entity::Entity()
{
	deleteRequested = false;
	addComponent(new Transform());
}


Entity::~Entity()
{
	for (unsigned int i = 0; i < components.size(); i++)
	{
		delete components.at(i);
	}
	components.clear();
}

void Entity::update(UpdatePackage * package)
{
	package->entity = this;
	for (unsigned int i = 0; i < components.size(); i++)
	{
		components.at(i)->update(package);
	}
	//delete any entities that need deleting
	for (unsigned int i = 0; i < components.size(); i++)
	{
		if (components.at(i)->isDeleteRequested())
		{
			components.erase(components.begin() + i);
			i--;
		}
	}
}

void Entity::render(GraphicsManager * graphics ,GraphicsCapture * capture)
{
	for (unsigned int i = 0; i < components.size(); i++)
	{
		components.at(i)->render(graphics, capture);
	}
}

void Entity::requestDelete()
{
	deleteRequested = true;
}

bool Entity::isDeleteRequested()
{
	return deleteRequested;
}

void Entity::addComponent(Component * component)
{
	components.push_back(component);
}

bool Entity::isContainerOfComponent(Component * component)
{
	for (unsigned int i = 0; i < components.size(); i++)
	{
		if (components.at(i) == component)
		{
			return true;
		}
	}
	return false;
}