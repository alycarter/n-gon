#pragma once
#include "emmet-engine\Component.h"
#include "CollisionListener.h"

#include <DirectXMath.h>
using namespace DirectX;

enum TURN_DIRECTION
{
	TURN_CLOCKWISE = -1, TURN_COUNTER_CLOCKWISE = 1, TURN_NONE = 0
};

class GraphicsManager;

class ShipController :
	public Component, public CollisionListener
{
public:
	ShipController(unsigned int health, float radius, float speed, float turnSpeed, float fireRate, float projectileRadius, XMFLOAT4 * colorIn);
	~ShipController();
	void update(UpdatePackage * package);
	void thrust();
	void turn(TURN_DIRECTION turn);
	void fire();
	void onCollide(Entity * entity);
	XMFLOAT4 * getColor();
private:
	void fireBullet(UpdatePackage * package);
	void kill(UpdatePackage * package);
	int health;
	float radius;
	float speed;
	float turnSpeed;
	float fireRate;
	float fireDelay;
	float projectileRadius;
	bool thrusting;
	bool firing;
	TURN_DIRECTION turnDirection;
	bool reDraw;
	unsigned int shader;
	unsigned int mesh;
	float particleDelay;
	float damageDelay;
	XMFLOAT4 color;
};

