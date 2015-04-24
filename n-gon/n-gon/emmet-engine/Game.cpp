#include "Game.h"
#include "StateMachine.h"
#include "Window.h"
#include "Controls.h"
#include "Time.h"
#include "GraphicsManager.h"
#include "UpdatePackage.h"

Game::Game()
{
	//set the name to game
	name = "Game";
}

Game::~Game()
{

}

void Game::play(State * initialState)
{
	stateMachine->requestPush(initialState);
	run();
	destroy();
}

void Game::run()
{
	do{
		update();
		render();
	} while (!stateMachine->isEmpty() && !window->isQuitRequested());
}

void Game::initialize(HINSTANCE hInstance, wchar_t * name, int width, int height)
{
	controls = new Controls();
	stateMachine = new StateMachine();
	window = new Window();
	window->create(name, 0, 0, width, height, hInstance);
	window->show();
	graphics.create(window, 1080, 1080);
	time = new Time(1.0f/5.0f);
	time->update();
}

void Game::update()
{
	time->update();
	//printf("delta time : %f run time: %f frame rate: %u \n", time->getDeltaTime(), time->getRunTime(), time->getFrameRate());
	controls->clearKeysReleased();
	window->readMessages(controls);
	UpdatePackage *  package = new UpdatePackage;
	ZeroMemory(package, sizeof(UpdatePackage));
	package->time = time;
	package->graphics = &graphics;
	package->controls = controls;
	stateMachine->update(package);
}

void Game::render()
{
	stateMachine->render(&graphics);
}

void Game::destroy()
{
	delete stateMachine;
}

GraphicsManager * Game::getGraphics()
{
	return &graphics;
}