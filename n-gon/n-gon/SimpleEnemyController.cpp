#include "SimpleEnemyController.h"
#include "emmet-engine\UpdatePackage.h"
#include "emmet-engine\Entity.h"
#include "emmet-engine\Time.h"

SimpleEnemyController::SimpleEnemyController()
{
	swapTime = 2.0f;
	swapDelay = swapTime / 2.0f;
	direction = TURN_CLOCKWISE;
}


SimpleEnemyController::~SimpleEnemyController()
{
}


void SimpleEnemyController::update(UpdatePackage * package)
{
	ShipController * ship = package->entity->getComponentOfType<ShipController>();
	ship->turn(direction);
	ship->fire();
	swapDelay -= package->time->getDeltaTime();
	if (swapDelay < 0)
	{
		swapDelay = swapTime;
		if (direction == TURN_CLOCKWISE)
		{
			direction = TURN_COUNTER_CLOCKWISE;
		}
		else
		{
			direction = TURN_CLOCKWISE;
		}
	}
}
