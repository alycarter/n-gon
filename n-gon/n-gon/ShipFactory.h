#pragma once
#include <DirectXMath.h>
using namespace DirectX;

enum SHIP_TYPE
{
	SHIP_TYPE_PLAYER, SHIP_TYPE_SPIN_ENEMY, SHIP_TYPE_AIMING_ENEMY, SHIP_TYPE_CHASE_ENEMY
};

class Entity;
class GraphicsManager;


class ShipFactory
{
public:
	ShipFactory();
	~ShipFactory();
	void createShip(Entity * ship, GraphicsManager * graphics, SHIP_TYPE type);
private:
	void createBaseShip(Entity * ship, unsigned int health, float radius, float speed, float turnSpeed, float fireRate, float projectileRadius, XMFLOAT4 * color, GraphicsManager * graphics);
};

