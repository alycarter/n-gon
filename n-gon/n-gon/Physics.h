#pragma once
#include "emmet-engine\Component.h"

#include <vector>
using namespace std;
#include <DirectXMath.h>
using namespace DirectX;

class CollisionListener;

class Physics :
	public Component
{
public:
	Physics();
	~Physics();
	void addVelocity(XMVECTOR * force);
	void addTorque(float force);
	XMVECTOR getVelocity();
	float getTorque();
	void update(UpdatePackage * package);
	void setColliderRadius(float radius);
	void addCollisionListener(CollisionListener * listener);
	void removeCollisionListener(CollisionListener * listener);
private:
	void notifyCollisionListeners(Entity * entity);
	XMFLOAT3 velocity;
	//rotation around the z axis
	float torque;
	float colliderRadius;
	vector<CollisionListener *> collisionListeners;
};

