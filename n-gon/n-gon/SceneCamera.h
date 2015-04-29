#pragma once
#include "emmet-engine\Component.h"
class SceneCamera :
	public Component
{
public:
	SceneCamera(float viewWidthIn, float viewHeightIn);
	~SceneCamera();
	void update(UpdatePackage * package);
	float getViewWidth();
	float getViewHeight();
private:
	float viewWidth;
	float viewHeight;
};

