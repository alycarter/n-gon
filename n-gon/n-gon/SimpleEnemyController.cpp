#include "SimpleEnemyController.h"
#include "emmet-engine\UpdatePackage.h"
#include "emmet-engine\Entity.h"
#include "emmet-engine\Time.h"

SimpleEnemyController::SimpleEnemyController()
{
	//time between rotation swaps
	swapTime = 2.0f;
	//time until next rotation swap
	swapDelay = swapTime / 2.0f;
	//starting rotation
	direction = TURN_CLOCKWISE;
}


SimpleEnemyController::~SimpleEnemyController()
{
}


void SimpleEnemyController::update(UpdatePackage * package)
{
	//get the ships controller
	ShipController * ship = package->entity->getComponentOfType<ShipController>();
	//turn the ship
	ship->turn(direction);
	//fire if possible
	ship->fire();
	//lower the time until the next swap
	swapDelay -= package->time->getDeltaTime();
	//if we need to swap
	if (swapDelay < 0)
	{
		//reset the swap timer
		swapDelay = swapTime;
		//flip the direction we're turning
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
