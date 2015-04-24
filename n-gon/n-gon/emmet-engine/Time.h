#pragma once
#include "Object.h"
#include <time.h>

class Time :
	public Object
{
public:
	Time(float maxDeltaTimeIn);
	~Time();
	void update();
	float getDeltaTime();
	float getRunTime();
	unsigned int getFrameRate();
private:
	float deltaTime;
	float runTime;
	clock_t startFrameTime;
	float maxDeltaTime;
};

