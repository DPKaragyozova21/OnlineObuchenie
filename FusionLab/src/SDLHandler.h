#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <cstdint>

#include <vector>

#include "Tile.h"

typedef SDL_Point Vector2;
typedef SDL_FPoint Vector2F;

typedef SDL_Rect Bounds2;
typedef SDL_FRect Bounds2F;

namespace fl::sdl
{

enum class SpriteEnum : uint16_t
{
	TILE_NONE,
	TILE_O2,
};

struct Sprite
{
	Bounds2 dstRect;
	SpriteEnum sprite;
};

class SDLHandler
{
public:

	SDLHandler(const char* title, const Vector2& res, const uint8_t& cameraZoom);

	~SDLHandler();

	void StartFrame() const;

	void EndFrame() const;

	void RenderSprite(const SpriteEnum& sprite, const Vector2& dstPoint) const;	

	const SDL_Event* GetInput();

private:


	SDL_Window* window;
	SDL_Renderer* renderer;

	SDL_Event* eventQueue;

	SDL_Texture* spritesheet;

	const uint8_t& zoom;

	mutable bool shouldRender;
};

}