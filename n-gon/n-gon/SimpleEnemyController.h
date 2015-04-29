#pragma once
#include "emmet-engine\Component.h"
#include "ShipController.h"

class SimpleEnemyController :
	public Component
{
public:
	SimpleEnemyController();
	~SimpleEnemyController();
	void update(UpdatePackage * package);
};

