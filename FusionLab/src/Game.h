#pragma once

#include "SDLHandler.h"
#include "Tile.h"
#include "Camera.h"
#include "Timer.h"
#include <array>

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

    void GenerateTerrain();

private:

    float deltaTime;
    Timer frameTimer;

    Vector2 resolution;

    Camera camera;
    sdl::SDLHandler sdlHandler;

    const SDL_Event* currentEvent;

    GameState state;

    bool shouldQuit;

    Vector2F mousePos;
    bool mouseButtonDown[2];

    std::array<Tile, 100000000> tiles;
    
};

}