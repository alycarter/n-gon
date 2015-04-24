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

ShipController::ShipController(unsigned int healthIn, float radiusIn, float speedIn, float turnSpeedIn, float fireRateIn, float projectileRadiusIn)
{
	health = healthIn;
	radius = radiusIn;
	speed = speedIn;
	turnSpeed = turnSpeedIn;
	fireRate = fireRateIn;
	fireDelay = 0;
	projectileRadius = projectileRadiusIn;
	turnDirection = TURN_CLOCKWISE;
	thrusting = false;
	firing = false;
	reDraw = true;
	particleDelay = 0;
	damageDelay = 0;
}

ShipController::~ShipController()
{
}

void ShipController::update(UpdatePackage * package)
{
	Physics * physics = package->entity->getComponentOfType<Physics>();
	Transform * transform = package->entity->getComponentOfType<Transform>();

	float change = package->time->getDeltaTime() * turnSpeed * (float)turnDirection;
	physics->addTorque(change * XMConvertToRadians(360.0f));
	if (physics->getTorque() < 0)
	{
		turnDirection = TURN_COUNTER_CLOCKWISE;
	}
	else
	{
		turnDirection = TURN_CLOCKWISE;
	}
	
	physics->addTorque(physics->getTorque() * - package->time->getDeltaTime());

	if (thrusting)
	{
		physics->addVelocity(&(transform->getUp() * speed * package->time->getDeltaTime()));
		thrusting = false;
		if (particleDelay < 0)
		{
			ParticleEmmiter * emmiter = package->entity->getComponentOfType<ParticleEmmiter>();
			XMVECTOR dir = -transform->getUp();
			dir = XMVector3Rotate(dir, XMQuaternionRotationRollPitchYaw(0, 0, XMConvertToRadians((rand() % 60) - 30.0f)));
			emmiter->emmitParticle(&(transform->getPosition() - (transform->getUp() * radius)), &((dir*100) + physics->getVelocity()));
			particleDelay = 1.0f / 75.0f;
		}
	}
	physics->addVelocity(&(physics->getVelocity() * -package->time->getDeltaTime()/4.0f));
	particleDelay -= package->time->getDeltaTime();
	if (firing)
	{
		Entity * bullet = new Entity();
		bullet->addComponent(new BulletController());
		Transform * bulletTran = bullet->getComponentOfType<Transform>();
		bulletTran->setPosition(&(transform->getPosition() + (transform->getUp() * (5+ radius + projectileRadius))));
		bulletTran->setRotation(&transform->getRotation());
		Physics * bulletPhysics = new Physics();
		bulletPhysics->addVelocity(&physics->getVelocity());
		bulletPhysics->addVelocity(&(transform->getUp() * 200));
		physics->addVelocity(&(transform->getUp() * -10));
		bulletPhysics->setColliderRadius(projectileRadius);
		bullet->addComponent(bulletPhysics);
		ShapeRenderer * renderer = new ShapeRenderer(package->graphics);
		renderer->buildShape(3, projectileRadius, 5, &XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f));
		bullet->addComponent(renderer);
		bulletPhysics->addCollisionListener(bullet->getComponentOfType<BulletController>());
		package->state->addEntity(bullet);
		fireDelay = fireRate;
		firing = false;
	}
	fireDelay -= package->time->getDeltaTime();

	if (health <= 0)
	{
		package->entity->requestDelete();
		package->entity->getComponentOfType<Physics>()->removeCollisionListener(this);
	}
	damageDelay -= package->time->getDeltaTime();

	if (reDraw)
	{
		ShapeRenderer * renderer = package->entity->getComponentOfType<ShapeRenderer>();
		if (renderer != NULL)
		{
			renderer->buildShape(health + 2, radius, 5, &XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
			LineInstance line;
			line.color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
			line.p1 = XMFLOAT3(0, 0, 0);
			line.p2 = XMFLOAT3(0, radius, 0);
			line.n1 = XMFLOAT3(-2.5f, 0, 0);
			line.n2 = XMFLOAT3(-2.5f, 0, 0);
			renderer->addLine(line);
		}
		physics->setColliderRadius(radius);
		reDraw = false;
	}
}

void ShipController::onCollide(Entity * entity)
{
	if (damageDelay <= 0)
	{
		health--;
		if (health > 0)
		{
			reDraw = true;
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