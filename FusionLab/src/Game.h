#pragma once

#include "SDLHandler.h"
#include "Tile.h"
#include "Camera.h"
#include "Timer.h"
#include "./Machines/Miner.h"
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

    void PlaceMachine();

    void DeleteMachine();

    void HandleEvent();

    void GenerateTerrain();

    Vector2 GetTileFromMousePos() const;

    void MachineLogicLoop();

protected:

    float fDeltaTime;
    Timer frameTimer;

    float lDeltaTime;
    Timer logicTimer;

    Vector2 resolution;

    Camera camera;
    sdl::SDLHandler sdlHandler;

    const SDL_Event* currentEvent;

    GameState state;

    std::atomic<bool> shouldQuit;

    Vector2F mousePos;
    bool mouseButtonDown[2];

    MachineType selectedMachine;
    uint8_t machineRotation;

    uint8_t tick;
    
    std::unordered_map<int, Machine*> machineMap;

    std::array<Tile, 100000000> tiles;
    
};

}