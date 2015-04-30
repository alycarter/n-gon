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
	//start by not turning
	turnDirection = 0;
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
	//add drag to the torque
	physics->addTorque(physics->getTorque() * - package->time->getDeltaTime() * 3.0f);

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
	
	//if the ship should fire then fire
	if (firing)
	{
		fireBullet(package);
	}
	
	//if the ship has no health
	if (health <= 0)
	{
		//kill the ship
		kill(package);
	}
	
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
	updateDelayTimers(package->time->getDeltaTime());
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

void ShipController::turn(float direction)
{
	//clamp direction to -1 and 1
	if (direction > 1)
	{
		direction = 1;
	}
	if (direction < -1)
	{
		direction = -1;
	}
	turnDirection = direction;
}

void ShipController::fire()
{
	//if we can fire
	if (fireDelay < 0)
	{
		//set the fire flag to true
		firing = true;
	}
}

void ShipController::fireBullet(UpdatePackage * package)
{
	//get the ships transform and physics
	Physics * physics = package->entity->getComponentOfType<Physics>();
	Transform * transform = package->entity->getComponentOfType<Transform>();
	//create a bullet entity
	Entity * bullet = new Entity();
	//attach a bullet controller
	bullet->addComponent(new BulletController());
	//get the bullets transform
	Transform * bulletTran = bullet->getComponentOfType<Transform>();
	//more the bullet to infront of the ship
	bulletTran->setPosition(&(transform->getPosition() + (transform->getUp() * (5 + radius + projectileRadius))));
	//set the rotatino to the players ship
	bulletTran->setRotation(&transform->getRotation());
	//make the bullets physics
	Physics * bulletPhysics = new Physics();
	//add the ships velocity to the bullet
	bulletPhysics->addVelocity(&physics->getVelocity());
	//make the bullet travel forwaard
	bulletPhysics->addVelocity(&(transform->getUp() * 200));
	//push the ship back slightly
	physics->addVelocity(&(transform->getUp() * -10));
	//set the bullet collider
	bulletPhysics->setColliderRadius(projectileRadius);
	//add the physics to the bullet
	bullet->addComponent(bulletPhysics);
	//set the shape renderer for the bullet
	ShapeRenderer * renderer = new ShapeRenderer(package->graphics);
	renderer->buildShape(3, projectileRadius, 5, &color);
	bullet->addComponent(renderer);
	bulletPhysics->addCollisionListener(bullet->getComponentOfType<BulletController>());
	//add the bullet
	package->state->addEntity(bullet);
	//reset the fire information
	fireDelay = fireRate;
	firing = false;
}

XMFLOAT4 * ShipController::getColor()
{
	return &color;
}

void ShipController::kill(UpdatePackage * package)
{
	//request the entities deletion
	package->entity->requestDelete();
	//remove the collision listener from the physiscs
	package->entity->getComponentOfType<Physics>()->removeCollisionListener(this);
	//make an explosion 
	Entity * explosion = new Entity();
	ParticleEmmiter * emmiter;
	explosion->addComponent(new ParticleEmmiter(package->graphics, 25));
	emmiter = explosion->getComponentOfType<ParticleEmmiter>();
	XMVECTOR pos = package->entity->getComponentOfType<Transform>()->getPosition();
	//spawn the explosion particles
	for (int i = 0; i < 25; i++)
	{
		XMVECTOR direction = XMVector3Rotate( XMVectorSet(1, 0, 0, 0), XMQuaternionRotationRollPitchYaw(0,0,XMConvertToRadians((float)(rand() % 360))));
		emmiter->emmitParticle(&(direction + pos),&(direction * 200.0f));
	}
	//add the explosion
	package->state->addEntity(explosion);

}

void ShipController::updateDelayTimers(float change)
{
	//lower the particle delay
	particleDelay -= change;
	//lower the bullet fire delay
	fireDelay -= change;
	//lower the damage delay
	damageDelay -= change;

}