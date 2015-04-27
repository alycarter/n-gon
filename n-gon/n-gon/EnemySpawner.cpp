#include "EnemySpawner.h"
#include "emmet-engine\UpdatePackage.h"
#include "emmet-engine\Time.h"
#include "emmet-engine\Transform.h"
#include "emmet-engine\State.h"
#include "emmet-engine\Entity.h"

EnemySpawner::EnemySpawner(Entity * enemyIn, float spawnTimeIn)
{
	enemy = enemyIn;
	spawnTime = spawnTimeIn;
	spawnDelay = spawnTimeIn;
}


EnemySpawner::~EnemySpawner()
{
}

void EnemySpawner::update(UpdatePackage * package)
{
	spawnDelay -= package->time->getDeltaTime();
	if (spawnDelay <= 0)
	{
		package->state->addEntity(enemy);
		package->entity->requestDelete();
		enemy->getComponentOfType<Transform>()->setPosition(&package->entity->getComponentOfType<Transform>()->getPosition());
	}
	else
	{
		float size = fmodf((spawnDelay / spawnTime)*500.0f, 100.0f);
		package->entity->getComponentOfType<Transform>()->setScale(&XMVectorSet(size, size, size, 0));
	}
}