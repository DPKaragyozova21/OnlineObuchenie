#include "SDLHandler.h"

#include <cstdlib>
#include <ctime>

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);

	fl::sdl::SDLHandler sdlHandler = {"e", 1920, 1080};

	srand((unsigned int)time(NULL));

	while (true)
	{
		sdlHandler.StartFrame();

		for (int i = 0; i < 200; i++)
		{
			sdlHandler.RenderSprite(fl::sdl::Sprite(rand() % 2), SDL_Point(129 * (i % 15), 129 * (i / 15)));
		}

		sdlHandler.EndFrame();

		SDL_Delay(500);
	}

	return 0;
}