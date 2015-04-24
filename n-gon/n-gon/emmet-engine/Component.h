#pragma once
#include "Object.h"

class Time;
class Controls;
class GraphicsManager;
class SubState;
class Entity;
class RenderPass;
class GraphicsCapture;

struct UpdatePackage;

class Component :
	public Object
{
public:
	Component();
	~Component();
	virtual void update(UpdatePackage * package){}
	virtual void render(GraphicsManager * graphics, GraphicsCapture * capture){}
	void requestDelete();
	bool isDeleteRequested();
private:
	bool deleteRequested;
};

