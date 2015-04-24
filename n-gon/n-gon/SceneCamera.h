#pragma once
#include "emmet-engine\Component.h"
class SceneCamera :
	public Component
{
public:
	SceneCamera();
	~SceneCamera();
	void update(UpdatePackage * package);
};

