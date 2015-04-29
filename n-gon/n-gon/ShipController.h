#pragma once
#include "emmet-engine\Component.h"
#include "CollisionListener.h"

#include <DirectXMath.h>
using namespace DirectX;

class GraphicsManager;

class ShipController :
	public Component, public CollisionListener
{
public:
	ShipController(unsigned int health, float radius, float speed, float turnSpeed, float fireRate, float projectileRadius, XMFLOAT4 * colorIn);
	~ShipController();
	void update(UpdatePackage * package);
	void thrust();
	void turn(float direction);
	void fire();
	void onCollide(Entity * entity);
	XMFLOAT4 * getColor();
private:
	void fireBullet(UpdatePackage * package);
	void kill(UpdatePackage * package);
	void updateDelayTimers(float change);
	int health;
	float radius;
	float speed;
	float turnSpeed;
	float fireRate;
	float fireDelay;
	float projectileRadius;
	bool thrusting;
	bool firing;
	float turnDirection;
	bool reDraw;
	unsigned int shader;
	unsigned int mesh;
	float particleDelay;
	float damageDelay;
	XMFLOAT4 color;
};

