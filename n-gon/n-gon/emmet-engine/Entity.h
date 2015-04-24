#pragma once
#include "Object.h"
#include <vector>
using namespace std;

class Time;
class Controls;
class GraphicsManager;
class SubState;
class Component;
class RenderPass;
class GraphicsCapture;
struct UpdatePackage;

class Entity :
	public Object
{
public:
	Entity();
	~Entity();
	void update(UpdatePackage * package);
	void render(GraphicsManager * graphics, GraphicsCapture * capture);
	void addComponent(Component * component);
	template < class T > void getComponentsOfType(vector<T *> * foundComponents);
	template < class T > T * getComponentOfType();
	template < class T > bool hasComponentOfType();
	bool isContainerOfComponent(Component * component);
	void requestDelete();
	bool isDeleteRequested();
private:
	vector<Component *> components;
	bool deleteRequested;
};

template < class T > void Entity::getComponentsOfType(vector<T *> * foundComponents)
{
	for (unsigned int i = 0; i < components.size(); i++)
	{
		if (dynamic_cast<T*>(components.at(i)) != NULL)
		{
			foundComponents->push_back((T*)components.at(i));
		}
	}
}

template < class T > T * Entity::getComponentOfType()
{
	for (unsigned int i = 0; i < components.size(); i++)
	{
		if (dynamic_cast<T*>(components.at(i)) != NULL)
		{
			return (T*)components.at(i);
		}
	}
	return NULL;
}

template < class T > bool Entity::hasComponentOfType()
{
	for (unsigned int i = 0; i < components.size(); i++)
	{
		if (dynamic_cast<T*>(components.at(i)) != NULL)
		{
			return true;
		}
	}
	return false;
}