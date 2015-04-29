#include "ShipController.h"
#include "emmet-engine\Entity.h"
#include "ShapeRenderer.h"
#include "emmet-engine\Transform.h"
#include "Physics.h"
#include "emmet-engine\UpdatePackage.h"
#include "emmet-engine\State.h"
#include "emmet-engine\Time.h"
#include "ParticleEmmiter.h"
#include "BulletController.h"

ShipController::ShipController(unsigned int healthIn, float radiusIn, float speedIn, float turnSpeedIn, float fireRateIn, float projectileRadiusIn, XMFLOAT4 * colorIn)
{
	//set ship stats
	health = healthIn;
	radius = radiusIn;
	speed = speedIn;
	turnSpeed = turnSpeedIn;
	fireRate = fireRateIn;
	//is ready to fire straight away
	fireDelay = 0;
	projectileRadius = projectileRadiusIn;
	//start by turning clockwise (change this to analogue input rather than digital)
	turnDirection = TURN_CLOCKWISE;
	//the ship is not thrusting
	thrusting = false;
	//the ship is not firing
	firing = false;
	//thr shap does need to be redrawn
	reDraw = true;
	//the ship can spawn a particle
	particleDelay = 0;
	//the ship can take damage
	damageDelay = 0;
	color = *colorIn;
}

ShipController::~ShipController()
{
}

void ShipController::update(UpdatePackage * package)
{
	//get information about the ships physics and trnasform
	Physics * physics = package->entity->getComponentOfType<Physics>();
	Transform * transform = package->entity->getComponentOfType<Transform>();

	//get the speed that the ship needs to turn at
	float change = package->time->getDeltaTime() * turnSpeed * (float)turnDirection;
	//turn the ship
	physics->addTorque(change * XMConvertToRadians(360.0f));
	//try to stabalize the ships tourque
	if (physics->getTorque() < 0)
	{
		turnDirection = TURN_COUNTER_CLOCKWISE;
	}
	else
	{
		turnDirection = TURN_CLOCKWISE;
	}
	//add drag to the torque
	physics->addTorque(physics->getTorque() * - package->time->getDeltaTime());

	//if the ship should thrust
	if (thrusting)
	{
		//get the velocity the ship should move
		physics->addVelocity(&(transform->getUp() * speed * package->time->getDeltaTime()));
		thrusting = false;
		//if we can spawn a particle
		if (particleDelay < 0)
		{
			//spawn a particle at the engine
			ParticleEmmiter * emmiter = package->entity->getComponentOfType<ParticleEmmiter>();
			XMVECTOR dir = -transform->getUp();
			dir = XMVector3Rotate(dir, XMQuaternionRotationRollPitchYaw(0, 0, XMConvertToRadians((rand() % 60) - 30.0f)));
			emmiter->emmitParticle(&(transform->getPosition() - (transform->getUp() * radius)), &((dir*100) + physics->getVelocity()));
			particleDelay = 1.0f / 75.0f;
		}
	}
	//apply drag to the velocity
	physics->addVelocity(&(physics->getVelocity() * -package->time->getDeltaTime()/4.0f));
	//lower the particle delay
	particleDelay -= package->time->getDeltaTime();
	
	//if the ship should fire then fire
	if (firing)
	{
		fireBullet(package);
	}
	//lower the bullet fire delay
	fireDelay -= package->time->getDeltaTime();

	//if the ship has no health
	if (health <= 0)
	{
		//kill the ship
		kill(package);
	}
	//lower the damage delay
	damageDelay -= package->time->getDeltaTime();

	//if the ship needs to redraw
	if (reDraw)
	{
		//get the shape renderer
		ShapeRenderer * renderer = package->entity->getComponentOfType<ShapeRenderer>();
		if (renderer != NULL)
		{
			//create a shape with the sides equal to the health + 2
			renderer->buildShape(health + 2, radius, 5, &color);
			//add a line representing the ships direction
			LineInstance line;
			line.color = color;
			line.p1 = XMFLOAT3(0, 0, 0);
			line.p2 = XMFLOAT3(0, radius, 0);
			line.n1 = XMFLOAT3(-2.5f, 0, 0);
			line.n2 = XMFLOAT3(-2.5f, 0, 0);
			renderer->addLine(line);
		}
		///////////////////////set the collider radius to the inner radius////////////////
		physics->setColliderRadius(radius);
		//the ship soesnt need to be redrawn any more
		reDraw = false;
	}
}

void ShipController::onCollide(Entity * entity)
{
	//if the ship can be damaged
	if (damageDelay <= 0)
	{
		//lower the health
		health--;
		//if the ship isnt dead
		if (health > 0)
		{
			//the ship should be redrawn
			reDraw = true;
			//set a small damage delay
			damageDelay = 0.1f;
		}
	}
}

void ShipController::thrust()
{
	thrusting = true;
}

void ShipController::turn(TURN_DIRECTION turn)
{
	turnDirection = turn;
}

void ShipController::fire()
{
	if (fireDelay < 0)
	{
		firing = true;
		fireDelay = fireRate;
	}
}

void ShipController::fireBullet(UpdatePackage * package)
{
	Physics * physics = package->entity->getComponentOfType<Physics>();
	Transform * transform = package->entity->getComponentOfType<Transform>();
	Entity * bullet = new Entity();
	bullet->addComponent(new BulletController());
	Transform * bulletTran = bullet->getComponentOfType<Transform>();
	bulletTran->setPosition(&(transform->getPosition() + (transform->getUp() * (5 + radius + projectileRadius))));
	bulletTran->setRotation(&transform->getRotation());
	Physics * bulletPhysics = new Physics();
	bulletPhysics->addVelocity(&physics->getVelocity());
	bulletPhysics->addVelocity(&(transform->getUp() * 200));
	physics->addVelocity(&(transform->getUp() * -10));
	bulletPhysics->setColliderRadius(projectileRadius);
	bullet->addComponent(bulletPhysics);
	ShapeRenderer * renderer = new ShapeRenderer(package->graphics);
	renderer->buildShape(3, projectileRadius, 5, &color);
	bullet->addComponent(renderer);
	bulletPhysics->addCollisionListener(bullet->getComponentOfType<BulletController>());
	package->state->addEntity(bullet);
	fireDelay = fireRate;
	firing = false;
}

XMFLOAT4 * ShipController::getColor()
{
	return &color;
}

void ShipController::kill(UpdatePackage * package)
{
	package->entity->requestDelete();
	package->entity->getComponentOfType<Physics>()->removeCollisionListener(this);
	Entity * explosion = new Entity();
	ParticleEmmiter * emmiter;
	explosion->addComponent(new ParticleEmmiter(package->graphics, 100));
	emmiter = explosion->getComponentOfType<ParticleEmmiter>();
	XMVECTOR pos = package->entity->getComponentOfType<Transform>()->getPosition();
	for (int i = 0; i < 100; i++)
	{
		XMVECTOR direction = XMVector3Rotate( XMVectorSet(1, 0, 0, 0), XMQuaternionRotationRollPitchYaw(0,0,XMConvertToRadians((float)(rand() % 360))));
		emmiter->emmitParticle(&(direction + pos),&(direction * 200.0f));
	}
	package->state->addEntity(explosion);

}