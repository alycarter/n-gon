#include "Time.h"


Time::Time(float maxDeltaTimeIn)
{
	deltaTime = 0;
	runTime = 0;
	startFrameTime = clock();
	maxDeltaTime = maxDeltaTimeIn;
}


Time::~Time()
{
}

void Time::update()
{
	
	clock_t endFrameTime = clock();
	deltaTime = (float)(endFrameTime - startFrameTime) / (float)CLOCKS_PER_SEC;
	startFrameTime = clock();
	runTime += deltaTime;
	if (deltaTime > maxDeltaTime)
	{
		deltaTime = maxDeltaTime;
	}
}

float Time::getDeltaTime()
{
	return deltaTime;
}

float Time::getRunTime()
{
	return runTime;
}

unsigned int Time::getFrameRate()
{
	return (unsigned int)(1.0f / deltaTime);
}