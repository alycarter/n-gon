#pragma once
#include "emmet-engine\Component.h"

class PlayerController :
	public Component
{
public:
	PlayerController();
	~PlayerController();
	void update(UpdatePackage * package);
private:
	float fireDelay;
};

