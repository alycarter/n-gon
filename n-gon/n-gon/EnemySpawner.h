#pragma once
#include "emmet-engine\Component.h"
class EnemySpawner :
	public Component
{
public:
	EnemySpawner(Entity * enemyIn, float spawnTimeIn);
	~EnemySpawner();
	void update(UpdatePackage * package);
private:
	Entity * enemy;
	float spawnDelay;
	float spawnTime;
};

