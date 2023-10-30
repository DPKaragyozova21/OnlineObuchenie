#include "Game.h"

int main(int argc, char* argv[])
{
	fl::Game* game = new fl::Game();

	game->Run();

	delete game;

	return 0;
}