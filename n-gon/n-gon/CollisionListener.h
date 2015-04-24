#pragma once

class Entity;

class CollisionListener
{
public:
	CollisionListener();
	~CollisionListener();
	virtual void onCollide(Entity * entity) = 0;
};

