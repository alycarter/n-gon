#pragma once
#include "emmet-engine\Component.h"

class ChaseEnemyController :
	public Component
{
public:
	ChaseEnemyController();
	~ChaseEnemyController();
	void update(UpdatePackage * package);
};

