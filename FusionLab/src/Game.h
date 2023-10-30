#pragma once

#include "SDLHandler.h"
#include "Tile.h"
#include "Camera.h"

namespace fl
{

class Game
{
private:

    enum class GameState
    {
        MAIN_MENU, IN_GAME
    };

public:

    Game();
    ~Game();

    void Run();

private:

    void RenderView();

    void HandleEvent();

private:

    Vector2 resolution;

    Camera camera;
    sdl::SDLHandler sdlHandler;

    const SDL_Event* currentEvent;

    GameState state;

    bool shouldQuit;

    Tile tiles[100000000];
    
};

}