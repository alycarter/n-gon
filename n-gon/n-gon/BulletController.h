#pragma once
#include "emmet-engine\Component.h"
#include "CollisionListener.h"

class BulletController :
	public Component, public CollisionListener
{
public:
	BulletController();
	~BulletController();
	void update(UpdatePackage * package);
	void onCollide(Entity * entity);
private:
	bool remove;
	float life;
};

