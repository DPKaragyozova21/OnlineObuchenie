#include "Game.h"

#include <random>
#include <iostream>

#define FPS 120.0f

namespace fl
{


Game::Game() :
    deltaTime(1000 / FPS),
    frameTimer(deltaTime),
    resolution({ 1280, 720 }),
    camera(resolution, deltaTime),
    sdlHandler({ "FusionLab", resolution, camera.zoom }),
    currentEvent(nullptr),
    state(GameState::IN_GAME),
    shouldQuit(false)
{
    GenerateTerrain();
}

Game::~Game()
{

}

void Game::Run()
{
    while (!shouldQuit)
    {
        frameTimer.GetDeltaTime();

        currentEvent = sdlHandler.GetInput();
        
        HandleEvent();

        //TO PUT IN DIFFERENT THREAD
        for (auto i = machineMap.begin(); i != machineMap.end(); i++)
        {
            if(i->second) i->second->Tick();
        }

        RenderView();
        camera.Update();

        if (mouseButtonDown[0]) PlaceMachine();
        
        if (1000 / FPS + deltaTime < 0) deltaTime = 0;
        SDL_Delay(1000 / FPS + deltaTime);
    }
}

void Game::RenderView()
{
    static sdl::SpriteEnum sprite;
    static int tilePos;
    const Bounds2F& camBounds = camera.GetView();
    
    sdlHandler.StartFrame();

    for (int i = -1; i <= camBounds.w; i++)
    {
        for (int j = -1; j <= camBounds.h; j++)
        {
            tilePos = (int)((floor(camBounds.x) + i) + ((floor(camBounds.y) + j) * 10000));
            if (tilePos >= 0 && tilePos < 100000000)
            {
                switch (tiles[tilePos].type)
                {
                case TileType::NONE:
                    sprite = sdl::SpriteEnum::TILE_NONE;
                    break;

                case TileType::O2:
                    sprite = sdl::SpriteEnum::TILE_O2;
                }
                sdlHandler.RenderSprite(sprite, { int(int(camBounds.x * 100) % 100 * (-0.32 * camera.zoom)) + int(i * (32 * camera.zoom)), int(int(camBounds.y * 100) % 100 * (-0.32 * camera.zoom)) + (j * int(32 * camera.zoom))});

                if (machineMap.find(tilePos) != machineMap.end() && machineMap[tilePos])
                {
                    sdlHandler.RenderSprite(machineMap[tilePos]->sprite, { int(int(camBounds.x * 100) % 100 * (-0.32 * camera.zoom)) + int(i * (32 * camera.zoom)), int(int(camBounds.y * 100) % 100 * (-0.32 * camera.zoom)) + (j * int(32 * camera.zoom)) });
                }
            }
        }
    }

    sdlHandler.EndFrame();
}

Vector2 Game::GetTileFromMousePos() const
{
    static Vector2F tilePos;
    tilePos = (camera.GetPosition() - ((resolution / 2) / camera.zoom)) + (mousePos / camera.zoom);

    tilePos.x = floor(tilePos.x / 32);
    tilePos.y = floor(tilePos.y / 32);

    return { (int)tilePos.x, (int)tilePos.y };
}

void Game::PlaceMachine()
{
    static Vector2 tile;
    static int tilePos;
    tile = GetTileFromMousePos();
    tilePos = tile.x + (tile.y * 10000);

    static bool input[4];
    static bool output[4];

    if (machineMap.find(tilePos) == machineMap.end() || !machineMap[tilePos])
    {
        switch (selectedMachine)
        {
        case MachineType::NONE:
            break;

        case MachineType::COVEYOR:
            break;

        case MachineType::MINER:
            if (tiles[tilePos].type != TileType::NONE)
            {
                machineMap[tilePos] = (Machine*)new Miner(tilePos, tiles[tilePos].type);
            }
            break;

        case MachineType::SPLITTER:
            /*machineMap[tile.x * tile.y * 10000] = new Machine();*/
            break;
        }
    }
    else return;
}

void Game::HandleEvent()
{
    if (!currentEvent) return;

    if (currentEvent->type == SDL_QUIT) shouldQuit = true;

    switch (state)
    {
    case GameState::MAIN_MENU:
        //TO DO
        break;

    case GameState::IN_GAME:
        switch (currentEvent->type)
        {
        case SDL_KEYDOWN:
            switch (currentEvent->key.keysym.sym)
            {
            case SDLK_w:
                camera.Move(Direction::UP, true);
                break;

            case SDLK_s:
                camera.Move(Direction::DOWN, true);
                break;

            case SDLK_a:
                camera.Move(Direction::LEFT, true);
                break;

            case SDLK_d:
                camera.Move(Direction::RIGHT, true);
                break;

            case SDLK_LSHIFT:
                camera.Sprint(true);
                break;

            case SDLK_1:
                selectedMachine = MachineType::COVEYOR;
                break;

            case SDLK_2:
                selectedMachine = MachineType::MINER;
                break;
            
            case SDLK_3:
                selectedMachine = MachineType::SPLITTER;
                break;
            }
            break;

        case SDL_KEYUP:
            switch (currentEvent->key.keysym.sym)
            {
            case SDLK_w:
                camera.Move(Direction::UP, false);
                break;

            case SDLK_s:
                camera.Move(Direction::DOWN, false);
                break;

            case SDLK_a:
                camera.Move(Direction::LEFT, false);
                break;

            case SDLK_d:
                camera.Move(Direction::RIGHT, false);
                break;

            case SDLK_LSHIFT:
                camera.Sprint(false);
                break;
            }
            break;
        
        case SDL_MOUSEWHEEL:
            if (currentEvent->wheel.y < 0) camera.ChangeZoom(false);
            if (currentEvent->wheel.y > 0) camera.ChangeZoom(true);
            break;

        case SDL_MOUSEMOTION:
            mousePos.x = currentEvent->motion.x;
            mousePos.y = currentEvent->motion.y;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (currentEvent->button.button == SDL_BUTTON_LEFT) mouseButtonDown[0] = true;
            if (currentEvent->button.button == SDL_BUTTON_RIGHT) mouseButtonDown[1] = true;
            break;
        case SDL_MOUSEBUTTONUP:
            if (currentEvent->button.button == SDL_BUTTON_LEFT) mouseButtonDown[0] = false;
            if (currentEvent->button.button == SDL_BUTTON_RIGHT) mouseButtonDown[1] = false;
        }
        break;

    }
}

void Game::GenerateTerrain()
{
    const int chunkSize = 8;

    std::default_random_engine generator;
    std::uniform_int_distribution<int> type(1, 1);
    std::uniform_int_distribution<int> oreChunk(1, 80);
    std::uniform_int_distribution<int> voidChance(0, 1);

    TileType currentChunkOreType = TileType::NONE;

    for (int i = 0; i < 100000000; i++)
    {
        tiles[i].type = TileType::NONE;
    }

    for (int i = 1; i < 9999 / chunkSize; i++)
    {
        for (int j = 1; j < 9999 / chunkSize; j++)
        {
            if (oreChunk(generator) == 1)
            {
                currentChunkOreType = TileType(type(generator));
            }
            else continue;

            for (int chunkI = 0; chunkI < chunkSize; chunkI++)
            {
                for (int chunkJ = 0; chunkJ < chunkSize; chunkJ++)
                {
                    if ((chunkJ == 0 || chunkJ == chunkSize - 1 || chunkI == 0 || chunkI == chunkSize - 1) && voidChance(generator) != 0) tiles[(i * chunkSize) + chunkI + ((j * chunkSize) + chunkJ) * 10000].type = TileType::NONE;
                    else tiles[(i * chunkSize) + chunkI + ((j * chunkSize) + chunkJ) * 10000].type = currentChunkOreType;
                    
                }
            }

            tiles[(i * chunkSize) + 0 + ((j * chunkSize) + chunkSize - 1) * 10000].type = TileType::NONE;
            tiles[(i * chunkSize) + 0 + ((j * chunkSize) + 0) * 10000].type = TileType::NONE;
            tiles[(i * chunkSize) + chunkSize - 1 + ((j * chunkSize) + chunkSize - 1) * 10000].type = TileType::NONE;
            tiles[(i * chunkSize) + chunkSize - 1 + ((j * chunkSize) + 0) * 10000].type = TileType::NONE;
        }
    }
}

}