#pragma once
#include "emmet-engine\Component.h"
class LevelController :
	public Component
{
public:
	LevelController();
	~LevelController();
	void update(UpdatePackage * package);
private:
	float shipSpawnDelay;
};

