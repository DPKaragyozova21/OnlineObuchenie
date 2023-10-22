#include "SDLHandler.h"

namespace fl::sdl
{

SDLHandler::SDLHandler(const char* title, const uint16_t& resX, const uint16_t& resY) :
	window(SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, resX, resY, 0)),
	renderer(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED)),
	event(new SDL_Event()),
	spritesheet(IMG_LoadTexture(renderer, "./FusionLab/assets/spritesheet.png")),
	shouldRender(false)
{

}

void SDLHandler::StartFrame()
{
	SDL_RenderClear(renderer);

	shouldRender = false;
}

void SDLHandler::EndFrame()
{
	if (shouldRender)
	{
		SDL_RenderPresent(renderer);
	}
}

void SDLHandler::RenderSprite(const Sprite& sprite, const SDL_Point& dstPoint)
{
	SDL_Rect srcRect;
	SDL_Rect dstRect = SDL_Rect(dstPoint.x, dstPoint.y, 128, 128);

	switch (sprite)
	{
	case Sprite::EMPTY_TILE:
		srcRect = { 0, 0, 32, 32 };
		break;

	case Sprite::O2_TILE:
		srcRect = { 32, 0, 32, 32 };
		break;
	}

	SDL_RenderCopy(renderer, spritesheet, &srcRect, &dstRect);

	shouldRender = true;
}

}