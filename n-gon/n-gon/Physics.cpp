#include "Physics.h"
#include "emmet-engine\Time.h"
#include "emmet-engine\Transform.h"
#include "emmet-engine\UpdatePackage.h"
#include "emmet-engine\Entity.h"
#include "emmet-engine\State.h"
#include "CollisionListener.h"
#include "SceneCamera.h"

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
	//add target velocity to the current velocity
	XMStoreFloat3(&velocity, getVelocity() + *force);
}

void Physics::addTorque(float force)
{
	//add the new torque
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
	//get the entities transform
	Transform * transform = package->entity->getComponentOfType<Transform>();
	//store the old position
	XMVECTOR oldPoint = transform->getPosition();
	//move to the new location
	XMVECTOR pos = transform->getPosition() + (getVelocity() * package->time->getDeltaTime());
	//wrap if it went off the screen
	SceneCamera * camera = package->state->getComponentOfType<SceneCamera>();
	if (XMVectorGetX(pos) + colliderRadius < 0)
	{
		pos += XMVectorSet(camera->getViewWidth() + (colliderRadius *2.0f),0,0,0);
	}
	else
	{
		if (XMVectorGetX(pos) - colliderRadius > camera->getViewWidth())
		{
			pos -= XMVectorSet(camera->getViewWidth() + (colliderRadius *2.0f), 0, 0, 0);
		}
	}
	if (XMVectorGetY(pos) + colliderRadius < 0)
	{
		pos += XMVectorSet(0, camera->getViewHeight() + (colliderRadius *2.0f), 0, 0);
	}
	else
	{
		if (XMVectorGetY(pos) - colliderRadius > camera->getViewHeight())
		{
			pos -= XMVectorSet(0, camera->getViewHeight() + (colliderRadius *2.0f), 0, 0);
		}
	}
	//set the new position
	transform->setPosition(&pos);
	//rotate the object
	transform->setRotation(&XMQuaternionMultiply(transform->getRotation(), XMQuaternionRotationAxis(XMVectorSet(0,0,1,0), torque * package->time->getDeltaTime())));
	//get a list of other physics objects in the scene
	vector<Physics *> colliders;
	package->state->getComponentsOfType<Physics>(&colliders);
	//test collision against them all
	for (unsigned int i = 0; i < colliders.size(); i++)
	{
		//if the collider isnt this object
		if (colliders.at(i) != this)
		{
			Entity * entity = package->state->getContainerEntity(colliders.at(i));
			Transform * targetTransform = entity->getComponentOfType<Transform>();
			//get the distance between the 2 objects
			float distance = XMVectorGetX(XMVector3Length(transform->getPosition() - targetTransform->getPosition()));
			// if the other object collided with this object
			if (distance < colliderRadius + colliders.at(i)->colliderRadius)
			{
				//reset our position
				transform->setPosition(&oldPoint);
				//tell all the collision listners about the collision
				notifyCollisionListeners(entity);
				colliders.at(i)->notifyCollisionListeners(package->entity);
				//bounce the objects
				XMVECTOR vel = getVelocity() - colliders.at(i)->getVelocity();
				colliders.at(i)->addVelocity(&(vel));
				addVelocity(&(-vel));
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
	//find the listner and remove it
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
	//call the collide event for all listeners
	for (unsigned int i = 0; i < collisionListeners.size(); i++)
	{
		collisionListeners.at(i)->onCollide(entity);
	}
}