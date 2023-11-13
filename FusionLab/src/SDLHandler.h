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

inline Vector2F operator+(const Vector2F& num1, const Vector2F& num2)
{
	static Vector2F finNum;

	finNum.x = num1.x + num2.x;
	finNum.y = num1.y + num2.y;

	return finNum;
}

inline Vector2F operator-(const Vector2F& num1, const Vector2& num2)
{
	static Vector2F finNum;

	finNum.x = num1.x - num2.x;
	finNum.y = num1.y - num2.y;

	return finNum;
}

inline Vector2F operator/(const Vector2F& num1, const float& num2)
{
	static Vector2F finNum;

	finNum.x = num1.x / num2;
	finNum.y = num1.y / num2;

	return finNum;
}

inline Vector2 operator/(const Vector2& num1, const float& num2)
{
	static Vector2 finNum;

	finNum.x = num1.x / num2;
	finNum.y = num1.y / num2;

	return finNum;
}

namespace fl::sdl
{

enum class SpriteEnum : uint16_t
{
	NONE,
	TILE_NONE,
	TILE_MAGNETITE,
	TILE_HEMATITE,
	MACHINE_MINER,
	MACHINE_CONVEYOR,
	MACHINE_CONVEYOR_LEFT,
	MACHINE_CONVEYOR_RIGHT,
	MACHINE_FURNACE,
	GHOST_MINER,
	GHOST_CONVEYOR
};

struct Sprite
{
	Bounds2 dstRect;
	SpriteEnum sprite;
	double rotation;
	uint8_t animationState;
};

class SDLHandler
{
public:

	SDLHandler(const char* title, const Vector2& res, const float& cameraZoom);

	~SDLHandler();

	void StartFrame() const;

	void EndFrame() const;

	void RenderSprite(const SpriteEnum& sprite, const Vector2& dstPoint, const double& rotation, const uint8_t& animationState) const;
	
	const SDL_Event* GetInput();

private:


	SDL_Window* window;
	SDL_Renderer* renderer;

	SDL_Event* eventQueue;

	SDL_Texture* spritesheet;

	const float& zoom;

	mutable bool shouldRender;
};

}