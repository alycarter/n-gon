#pragma once
#include "emmet-engine\Component.h"

class AimingEnemyController :
	public Component
{
public:
	AimingEnemyController();
	~AimingEnemyController();
	void update(UpdatePackage * package);
};

