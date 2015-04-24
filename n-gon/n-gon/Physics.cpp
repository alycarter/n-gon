#include "Physics.h"
#include "emmet-engine\Time.h"
#include "emmet-engine\Transform.h"
#include "emmet-engine\UpdatePackage.h"
#include "emmet-engine\Entity.h"
#include "emmet-engine\State.h"
#include "emmet-engine\GraphicsManager.h"
#include "CollisionListener.h"

Physics::Physics()
{
	velocity = XMFLOAT3(0, 0, 0);
	torque = 0;
	colliderRadius = 0;
}


Physics::~Physics()
{
}

void Physics::addVelocity(XMVECTOR * force)
{
	XMStoreFloat3(&velocity, getVelocity() + *force);
}

void Physics::addTorque(float force)
{
	torque += force;
}

XMVECTOR Physics::getVelocity()
{
	return XMLoadFloat3(&velocity);
}

float Physics::getTorque()
{
	return torque;
}

void Physics::update(UpdatePackage * package)
{
	Transform * transform = package->entity->getComponentOfType<Transform>();
	XMVECTOR oldPoint = transform->getPosition();
	XMVECTOR pos = transform->getPosition() + (getVelocity() * package->time->getDeltaTime());
	if (XMVectorGetX(pos) + colliderRadius < 0)
	{
		pos += XMVectorSet(package->graphics->getScreenResolution()->x + (colliderRadius *2.0f),0,0,0);
	}
	else
	{
		if (XMVectorGetX(pos) - colliderRadius > package->graphics->getScreenResolution()->x)
		{
			pos -= XMVectorSet(package->graphics->getScreenResolution()->x + (colliderRadius *2.0f), 0, 0, 0);
		}
	}
	if (XMVectorGetY(pos) + colliderRadius < 0)
	{
		pos += XMVectorSet(0, package->graphics->getScreenResolution()->y + (colliderRadius *2.0f), 0, 0);
	}
	else
	{
		if (XMVectorGetY(pos) - colliderRadius > package->graphics->getScreenResolution()->y)
		{
			pos -= XMVectorSet(0, package->graphics->getScreenResolution()->y + (colliderRadius *2.0f), 0, 0);
		}
	}
	transform->setPosition(&pos);
	transform->setRotation(&XMQuaternionMultiply(transform->getRotation(), XMQuaternionRotationAxis(XMVectorSet(0,0,1,0), torque * package->time->getDeltaTime())));
	vector<Physics *> colliders;
	package->state->getComponentsOfType<Physics>(&colliders);
	for (unsigned int i = 0; i < colliders.size(); i++)
	{
		if (colliders.at(i) != this)
		{
			Entity * entity = package->state->getContainerEntity(colliders.at(i));
			Transform * targetTransform = entity->getComponentOfType<Transform>();
			float distance = XMVectorGetX(XMVector3Length(transform->getPosition() - targetTransform->getPosition()));
			if (distance < colliderRadius + colliders.at(i)->colliderRadius)
			{
				transform->setPosition(&oldPoint);
				notifyCollisionListeners(entity);
				colliders.at(i)->notifyCollisionListeners(package->entity);
				XMVECTOR vel = getVelocity() - colliders.at(i)->getVelocity();
				colliders.at(i)->addVelocity(&(vel / 4.0f));
				addVelocity(&(-vel*1.25f));
			}
		}
	}
}

void Physics::setColliderRadius(float radius)
{
	colliderRadius = radius;
}

void Physics::addCollisionListener(CollisionListener * listener)
{
	collisionListeners.push_back(listener);
}

void Physics::removeCollisionListener(CollisionListener * listener)
{
	for (unsigned int i = 0; i < collisionListeners.size(); i++)
	{
		if (collisionListeners.at(i) == listener)
		{
			collisionListeners.erase(collisionListeners.begin() + i);
			i--;
		}
	}
}

void Physics::notifyCollisionListeners(Entity * entity)
{
	for (unsigned int i = 0; i < collisionListeners.size(); i++)
	{
		collisionListeners.at(i)->onCollide(entity);
	}
}