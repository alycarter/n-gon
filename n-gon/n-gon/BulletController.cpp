#include "BulletController.h"
#include "emmet-engine\UpdatePackage.h"
#include "emmet-engine\Entity.h"
#include "emmet-engine\Time.h"
#include "Physics.h"

BulletController::BulletController()
{
	life = 4;
}


BulletController::~BulletController()
{
}

void BulletController::update(UpdatePackage * package)
{
	//lower the life count
	life -= package->time->getDeltaTime();
	//if the bullet has collided or ran out of life then delete it
	if (remove || life < 0)
	{
		package->entity->requestDelete();
		package->entity->getComponentOfType<Physics>()->removeCollisionListener(this);
	}
}

void BulletController::onCollide(Entity * entity)
{
	remove = true;
}