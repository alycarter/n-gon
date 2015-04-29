#include "SimpleEnemyController.h"
#include "emmet-engine\UpdatePackage.h"
#include "emmet-engine\Entity.h"
#include "emmet-engine\Time.h"

SimpleEnemyController::SimpleEnemyController()
{
}


SimpleEnemyController::~SimpleEnemyController()
{
}


void SimpleEnemyController::update(UpdatePackage * package)
{
	//get the ships controller
	ShipController * ship = package->entity->getComponentOfType<ShipController>();
	//turn the ship
	ship->turn(cos(package->time->getRunTime()));
	//fire if possible
	ship->fire();
}
