#pragma once
#include "Object.h"
#include <vector>
using namespace std;

class Time;
class Controls;
class GraphicsManager;
class GraphicsCapture;
class SubState;
class Entity;
class Component;

struct UpdatePackage;

class State :
	public Object
{
public:
	State();
	~State();
	void update(UpdatePackage * package);
	void render(GraphicsManager * graphics);
	template < class T > void getEntitiesWithComponentOfType(vector<Entity *> * foundEntities);
	template < class T > void getComponentsOfType(vector<T *> * foundComponents);
	template < class T > T * getComponentOfType();
	Entity * getContainerEntity(Component * component);
	void addEntity(Entity * entity);
	GraphicsCapture * getFrontCaptureBuffer();
	void swapCaptureBuffers();
	void addRenderPass(char * name);
	bool isOccluding();
	void setOccluding(bool occlude);
private:
	vector<Entity*> entities;
	GraphicsCapture * frontCaptureBuffer;
	GraphicsCapture * backCaptureBuffer;
	bool occluding;
};

template < class T > void State::getEntitiesWithComponentOfType(vector<Entity *> * foundEntities)
{
	for (unsigned int i = 0; i < entities.size(); i++)
	{
		if (entities.at(i)->hasComponentOfType<T>())
		{
			foundEntities->push_back(entities.at(i));
		}
	}
}
template < class T > void State::getComponentsOfType(vector<T *> * foundComponents)
{
	for (unsigned int i = 0; i < entities.size(); i++)
	{
		entities.at(i)->getComponentsOfType<T>(foundComponents);
	}
}
template < class T > T * State::getComponentOfType()
{
	for (unsigned int i = 0; i < entities.size(); i++)
	{
		if (entities.at(i)->hasComponentOfType<T>())
		{
			return entities.at(i)->getComponentOfType<T>();
		}
	}
	return NULL;
}