#pragma once
#include "emmet-engine\State.h"

class GraphicsManager;

class Scene :
	public State
{
public:
	Scene(GraphicsManager * manager);
	~Scene();
};

