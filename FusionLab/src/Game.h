#pragma once

#include "SDLHandler.h"
#include "Tile.h"
#include "Camera.h"
#include "Timer.h"
#include "Hub.h"
#include "./Machines/Miner.h"
#include "./Machines/Conveyor.h"
#include "./Machines/Furnace.h"
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

    Vector2 GetTileFromMousePos() const;

    void PlaceMachine();

    void DeleteMachine();

    void SampleMachine();

    void HandleEvent();

    void GenerateTerrain();

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
    bool invertRotation;

    uint8_t conveyorState;

    uint8_t tick;
    
    std::unordered_map<int, Machine*> machineMap;

    Hub* hub;

    std::array<Tile, 1000000> tiles;
    
};

}