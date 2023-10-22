#pragma once

#include <SDL.h>
#include <SDL_image.h>

namespace fl::sdl
{

enum class Sprite : uint8_t
{
	EMPTY_TILE,
	O2_TILE
};

class SDLHandler
{
public:

	SDLHandler(const char* title, const uint16_t& resX, const uint16_t& resY);

	void StartFrame();

	void EndFrame();

	void RenderSprite(const Sprite& sprite, const SDL_Point& dstPoint);

public:

	

private:

	SDL_Window* window;
	SDL_Renderer* renderer;

	SDL_Event* event;

	SDL_Texture* spritesheet;

	bool shouldRender;
};

}