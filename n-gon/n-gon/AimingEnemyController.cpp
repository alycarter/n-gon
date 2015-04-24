#include "AimingEnemyController.h"
#include "ShipController.h"
#include "emmet-engine\Transform.h"
#include "emmet-engine\State.h"
#include "emmet-engine\UpdatePackage.h"
#include "emmet-engine\Entity.h"
#include "Physics.h"

AimingEnemyController::AimingEnemyController()
{
}


AimingEnemyController::~AimingEnemyController()
{
}

void AimingEnemyController::update(UpdatePackage * package)
{
	PlayerController * playerController = package->state->getComponentOfType<PlayerController>();
	if (playerController != NULL)
	{
		Entity * player = package->state->getContainerEntity(playerController);
		ShipController * ship = package->entity->getComponentOfType<ShipController>();
		Transform * transform = package->entity->getComponentOfType<Transform>();
		Physics * physics = package->entity->getComponentOfType<Physics>();
		Transform * playerTransform = player->getComponentOfType<Transform>();
		XMVECTOR difference = transform->getPosition() - playerTransform->getPosition();
		float distance = XMVectorGetX(XMVector3Length(difference));
		difference = XMVector3Normalize(difference);
		XMVECTOR cross = XMVector3Cross(transform->getUp(), difference);
		if (XMVectorGetZ(cross) > 0)
		{
			ship->turn(TURN_CLOCKWISE);
		}
		else
		{
			ship->turn(TURN_COUNTER_CLOCKWISE);
		}
		if (abs(XMVectorGetZ(cross)) < 0.1f)
		{
			ship->fire();
		}
		if (distance > 200 && abs(abs(XMVectorGetZ(cross)) < 0.5f))
		{
			ship->thrust();
		}
	}
}