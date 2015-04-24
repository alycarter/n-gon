#include <windows.h>
#include <stdio.h>
#include "emmet-engine\Game.h"
#include "Scene.h"

#define DEBUG true

// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	FILE * stream;
	if (DEBUG)
	{
		AllocConsole();
		AttachConsole(GetCurrentProcessId());
		freopen_s(&stream, "CONOUT$", "w", stdout);
	}
	//create a game
	Game game;
	//play the game withe the window size and name info
	game.initialize(hInstance, L"N-GON", 1280, 720);
	Scene * scene = new Scene(game.getGraphics());
	game.play(scene);
	if (DEBUG)
	{
		fclose(stream);
	}
	return 0;
}