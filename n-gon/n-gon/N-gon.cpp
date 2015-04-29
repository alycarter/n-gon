#include <windows.h>
#include <stdio.h>
#include "emmet-engine\Game.h"
#include "Scene.h"

#define DEBUG true

// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	FILE * stream;
	//if in debug mode then create a console
	if (DEBUG)
	{
		AllocConsole();
		AttachConsole(GetCurrentProcessId());
		freopen_s(&stream, "CONOUT$", "w", stdout);
	}
	//create a game
	Game game;
	//play the game withe the window size and name info
	game.initialize(hInstance, L"N-GON", 1920, 1080);
	//create the base state
	Scene * scene = new Scene(game.getGraphics());
	//play the game with the scene state
	game.play(scene);
	//if in debug close the console
	if (DEBUG)
	{
		fclose(stream);
	}
	return 0;
}