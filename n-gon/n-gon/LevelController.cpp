#include "LevelController.h"
#include "emmet-engine\UpdatePackage.h"
#include "emmet-engine\Time.h"
#include "emmet-engine\Entity.h"
#include "emmet-engine\State.h"
#include "emmet-engine\Transform.h"
#include "emmet-engine\Controls.h"
#include "emmet-engine\StateMachine.h"
#include "ShipFactory.h"
#include "EnemySpawner.h"
#include "ShapeRenderer.h"
#include "ShipController.h"
#include "SceneCamera.h"


LevelController::LevelController()
{
	shipSpawnDelay = 5;
}


LevelController::~LevelController()
{
}

void LevelController::update(UpdatePackage * package)
{
	//lower the ship spawn delay
	shipSpawnDelay -= package->time->getDeltaTime();
	//if the spawn timer ran out then spwan a ship
	if (shipSpawnDelay < 0)
	{
		//reset the spawn timer
		shipSpawnDelay = 5;
		ShipFactory factory;
		Entity * entity = new Entity();
		//pick a ship to spawn
		int shipType = rand() % 3;
		//spawn the ship
		if (shipType == 0)
		{
			factory.createShip(entity, package->graphics, SHIP_TYPE_CHASE_ENEMY);
		}
		if (shipType == 1)
		{
			factory.createShip(entity, package->graphics, SHIP_TYPE_AIMING_ENEMY);
		}
		if (shipType == 2)
		{
			factory.createShip(entity, package->graphics, SHIP_TYPE_SPIN_ENEMY);
		}
		//create a spawner for the ship
		Entity * spawner = new Entity();
		//place the spawner in a random position
		SceneCamera * cam = package->state->getComponentOfType<SceneCamera>();
		spawner->getComponentOfType<Transform>()->setPosition(&XMVectorSet((float)(rand() % (int)cam->getViewWidth()), (float)(rand() % (int)cam->getViewHeight()), 0, 0));
		spawner->addComponent(new EnemySpawner(entity, 3));
		spawner->addComponent(new ShapeRenderer(package->graphics));
		spawner->getComponentOfType<ShapeRenderer>()->buildShape(10, 1, 0.1f, entity->getComponentOfType<ShipController>()->getColor());
		package->state->addEntity(spawner);
	}
	//if escape if pressed then quit
	if (package->controls->isKeyReleased(VK_ESCAPE))
	{
		package->stateMachine->requestPop();
	}
}
