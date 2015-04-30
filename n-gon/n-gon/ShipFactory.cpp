#include "ShipFactory.h"
#include "emmet-engine\Entity.h"
#include "Physics.h"
#include "ShipController.h"
#include "ParticleEmmiter.h"
#include "ShapeRenderer.h"
#include "PlayerController.h"
#include "SimpleEnemyController.h"
#include "AimingEnemyController.h"
#include "ChaseEnemyController.h"

ShipFactory::ShipFactory()
{

}


ShipFactory::~ShipFactory()
{

}

void ShipFactory::createShip(Entity * ship, GraphicsManager * graphics, SHIP_TYPE type)
{
	//make a different ship depending on the given ship type
	switch (type)
	{
	case SHIP_TYPE_PLAYER:
		createBaseShip(ship, 10, 15, 150, XMConvertToRadians(90), 0.25f, 5, &XMFLOAT4(0,1,0,1), graphics);
		ship->addComponent(new PlayerController());
		break;
	case SHIP_TYPE_SPIN_ENEMY:
		createBaseShip(ship, 3, 15, 100, XMConvertToRadians(360), 1.0f, 5, &XMFLOAT4(1, 0, 1, 1), graphics);
		ship->addComponent(new SimpleEnemyController());
		break;
	case SHIP_TYPE_CHASE_ENEMY:
		createBaseShip(ship, 3, 15, 100, XMConvertToRadians(90), 1.0f, 5, &XMFLOAT4(1, 1, 0, 1), graphics);
		ship->addComponent(new ChaseEnemyController());
		break;
	case SHIP_TYPE_AIMING_ENEMY:
		createBaseShip(ship, 3, 15, 100, XMConvertToRadians(90), 1.0f, 5, &XMFLOAT4(1, 0, 0, 1), graphics);
		ship->addComponent(new AimingEnemyController());
		break;
	default:
		break;
	}
}


void ShipFactory::createBaseShip(Entity * ship, unsigned int health, float radius, float speed, float turnSpeed, float fireRate, float projectileRadius, XMFLOAT4 * color, GraphicsManager * graphics)
{
	//add the parts needed for a ship and connect the collision listener
	ship->addComponent(new Physics());
	ship->addComponent(new ShipController(health, radius, speed, turnSpeed, fireRate, projectileRadius, color));
	ship->addComponent(new ShapeRenderer(graphics));
	ship->addComponent(new ParticleEmmiter(graphics, 100));
	ship->getComponentOfType<Physics>()->addCollisionListener(ship->getComponentOfType<ShipController>());
}
