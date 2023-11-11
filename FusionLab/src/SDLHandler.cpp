#include "SDLHandler.h"

#include <algorithm>

namespace fl::sdl
{

inline std::vector<Sprite> toRender;

bool compareSprites(const Sprite& sprite1, const Sprite& sprite2)
{
	return uint16_t(sprite1.sprite) < uint16_t(sprite2.sprite);
}

SDLHandler::SDLHandler(const char* title, const Vector2& res, const float& cameraZoom) :
	window(nullptr),
	renderer(nullptr),
	eventQueue(nullptr),
	spritesheet(nullptr),
	zoom(cameraZoom),
	shouldRender(false)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);

	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, res.x, res.y, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	
	eventQueue = new SDL_Event();

	spritesheet = IMG_LoadTexture(renderer, "./FusionLab/assets/spritesheet.png");
}

SDLHandler::~SDLHandler()
{
	SDL_DestroyTexture(spritesheet);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	delete eventQueue;

	SDL_Quit();
}

void SDLHandler::StartFrame() const
{
	SDL_RenderClear(renderer);

	shouldRender = false;
}

void SDLHandler::EndFrame() const
{
	static SDL_Rect srcRect;
	if (shouldRender)
	{
		std::sort(toRender.begin(), toRender.end(), compareSprites);
		
		for (const Sprite& sprite : toRender)
		{
			srcRect = {((uint16_t)sprite.sprite * 32) % 2048, ((uint16_t)sprite.sprite * 32) / 2048, 32, 32 };
			SDL_RenderCopy(renderer, spritesheet, &srcRect, &sprite.dstRect);
		}

		SDL_RenderPresent(renderer);

		toRender.clear();
	}
}

void SDLHandler::RenderSprite(const SpriteEnum& sprite, const Vector2& dstPoint) const
{
	toRender.push_back(Sprite({ SDL_Rect(dstPoint.x, dstPoint.y, 32 * zoom, 32 * zoom), sprite}));

	shouldRender = true;
}

const SDL_Event* SDLHandler::GetInput()
{
	SDL_PollEvent(eventQueue);

	return eventQueue;
}

}