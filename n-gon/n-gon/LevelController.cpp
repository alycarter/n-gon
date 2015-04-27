#include "LevelController.h"
#include "emmet-engine\UpdatePackage.h"
#include "emmet-engine\Time.h"
#include "emmet-engine\Entity.h"
#include "emmet-engine\State.h"
#include "emmet-engine\Transform.h"
#include "ShipFactory.h"
#include "EnemySpawner.h"
#include "ShapeRenderer.h"
#include "ShipController.h"


LevelController::LevelController()
{
	shipSpawnDelay = 5;
}


LevelController::~LevelController()
{
}

void LevelController::update(UpdatePackage * package)
{
	shipSpawnDelay -= package->time->getDeltaTime();
	if (shipSpawnDelay < 0)
	{
		shipSpawnDelay = 5;
		ShipFactory factory;
		Entity * entity = new Entity();
		if (rand() % 2 == 0)
		{
			factory.createShip(entity, package->graphics, SHIP_TYPE_CHASE_ENEMY);
		}
		else
		{
			factory.createShip(entity, package->graphics, SHIP_TYPE_SPIN_ENEMY);
		}
		Entity * spawner = new Entity();
		spawner->getComponentOfType<Transform>()->setPosition(&XMVectorSet((float)(rand() % 1280), (float)(rand() % 720), 0, 0));
		spawner->addComponent(new EnemySpawner(entity, 3));
		spawner->addComponent(new ShapeRenderer(package->graphics));
		spawner->getComponentOfType<ShapeRenderer>()->buildShape(10, 1, 0.1f, entity->getComponentOfType<ShipController>()->getColor());
		package->state->addEntity(spawner);
	}
}
