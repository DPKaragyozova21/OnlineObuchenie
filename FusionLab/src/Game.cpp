#include "Game.h"

#include <random>
#include <iostream>

#define FPS 120.0f

namespace fl
{


Game::Game() :
    fDeltaTime(1000 / FPS),
    frameTimer(fDeltaTime),
    lDeltaTime(0),
    logicTimer(lDeltaTime),
    resolution({ 1280, 720 }),
    camera(resolution, fDeltaTime),
    sdlHandler({ "FusionLab", resolution, camera.zoom }),
    currentEvent(nullptr),
    state(GameState::IN_GAME),
    shouldQuit(false),
    mousePos({ 0, 0 }),
    mouseButtonDown{0, 0},
    selectedMachine(MachineType::NONE),
    machineRotation(0),
    invertRotation(false),
    conveyorState(0),
    tick(0),
    machineMap(),
    hub(nullptr)
{
    GenerateTerrain();
    Machine::SetMap(&machineMap);

    hub = new Hub();
    for (int i = 0; i < 16; i++)
    {
        if (hub->parts[i]) machineMap[hub->parts[i]->pos] = hub->parts[i];
    }
}

Game::~Game()
{
    delete hub;

    for (auto i : machineMap)
    {
        delete i.second;
    }

    machineMap.clear();
}

void Game::Run()
{
    std::jthread* logicThread = new std::jthread(&Game::MachineLogicLoop, this);

    while (!shouldQuit.load())
    {

        currentEvent = sdlHandler.GetInput();
        
        HandleEvent();

        RenderView();
        camera.Update();

        hub->TransferItemsToHub();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(int(2)));
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
            tilePos = (int)((floor(camBounds.x) + i) + ((floor(camBounds.y) + j) * 1000));
            if (tilePos >= 0 && tilePos < 1000000)
            {
                switch (tiles[tilePos].type)
                {
                case TileType::NONE:
                    sprite = sdl::SpriteEnum::TILE_NONE;
                    break;

                case TileType::MAGNETITE:
                    sprite = sdl::SpriteEnum::TILE_MAGNETITE;
                    break;

                case TileType::HEMATITE:
                    sprite = sdl::SpriteEnum::TILE_HEMATITE;
                    break;
                }
                sdlHandler.RenderSprite(sprite, { int(int(camBounds.x * 100) % 100 * (-0.32 * camera.zoom)) + int(i * (32 * camera.zoom)), int(int(camBounds.y * 100) % 100 * (-0.32 * camera.zoom)) + (j * int(32 * camera.zoom))}, 0, 0);

                if (machineMap.find(tilePos) != machineMap.end() && machineMap[tilePos])
                {
                    if(machineMap[tilePos]->type == MachineType::CONVEYOR) sdlHandler.RenderSprite(machineMap[tilePos]->sprite, { int(int(camBounds.x * 100) % 100 * (-0.32 * camera.zoom)) + int(i * (32 * camera.zoom)), int(int(camBounds.y * 100) % 100 * (-0.32 * camera.zoom)) + (j * int(32 * camera.zoom)) }, machineMap[tilePos]->rotation * 90, conveyorState);
                    else sdlHandler.RenderSprite(machineMap[tilePos]->sprite, { int(int(camBounds.x * 100) % 100 * (-0.32 * camera.zoom)) + int(i * (32 * camera.zoom)), int(int(camBounds.y * 100) % 100 * (-0.32 * camera.zoom)) + (j * int(32 * camera.zoom)) }, machineMap[tilePos]->rotation * 90, machineMap[tilePos]->animationState);
                }
            }
        }
    }

    static sdl::SpriteEnum machineGhost;

    switch (selectedMachine)
    {
    case MachineType::NONE:
        machineGhost = sdl::SpriteEnum::NONE;
        break;

    case MachineType::CONVEYOR:
        machineGhost = sdl::SpriteEnum::GHOST_CONVEYOR;
        break;

    case MachineType::MINER:
        machineGhost = sdl::SpriteEnum::GHOST_MINER;
        break;

    case MachineType::FURNACE:
        machineGhost = sdl::SpriteEnum::MACHINE_FURNACE;
        break;
    }

    if (selectedMachine != MachineType::NONE) sdlHandler.RenderSprite(machineGhost, { int(mousePos.x - (16 * camera.zoom)), int(mousePos.y - (16 * camera.zoom)) }, machineRotation * 90, 0);

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
    tilePos = tile.x + (tile.y * 1000);

    if (machineMap.find(tilePos) == machineMap.end() || !machineMap[tilePos])
    {
        switch (selectedMachine)
        {
        case MachineType::NONE:
            break;

        case MachineType::CONVEYOR:
            machineMap[tilePos] = (Machine*)new Conveyor(tilePos, tiles[tilePos].type, machineRotation);
            break;

        case MachineType::MINER:
            if (tiles[tilePos].type != TileType::NONE)
            {
                machineMap[tilePos] = (Machine*)new Miner(tilePos, tiles[tilePos].type, machineRotation);
            }
            break;

        case MachineType::FURNACE:
            machineMap[tilePos] = (Machine*)new Furnace(tilePos, tiles[tilePos].type, machineRotation);
            break;
        }
    }
    else return;
}

void Game::DeleteMachine()
{
    static Vector2 tile;
    static int tilePos;
    tile = GetTileFromMousePos();
    tilePos = tile.x + (tile.y * 1000);

    if (machineMap.find(tilePos) != machineMap.end() && machineMap[tilePos]->type != MachineType::HUB_INPUT)
    {
        delete machineMap[tilePos];
        machineMap.erase(tilePos);
        if (machineMap.find(tilePos - 1) != machineMap.end() && machineMap[tilePos - 1])
        {
            machineMap[tilePos - 1]->ResetIO();
            machineMap[tilePos - 1]->UpdateIO();
        }
        if (machineMap.find(tilePos + 1) != machineMap.end() && machineMap[tilePos + 1])
        {
            machineMap[tilePos + 1]->ResetIO();
            machineMap[tilePos + 1]->UpdateIO();
        }
        if (machineMap.find(tilePos - 1000) != machineMap.end() && machineMap[tilePos - 1000])
        {
            machineMap[tilePos - 1000]->ResetIO();
            machineMap[tilePos - 1000]->UpdateIO();
        }
        if (machineMap.find(tilePos + 1000) != machineMap.end() && machineMap[tilePos + 1000])
        {
            machineMap[tilePos + 1000]->ResetIO();
            machineMap[tilePos + 1000]->UpdateIO();
        }
    }
}

void Game::SampleMachine()
{
    static int tilePos;
    tilePos = GetTileFromMousePos().x + GetTileFromMousePos().y * 1000;

    if (machineMap.find(tilePos) != machineMap.end() && machineMap[tilePos])
    {
        selectedMachine = machineMap[tilePos]->type;
    }
}

void Game::MachineLogicLoop()
{
    while (!shouldQuit.load())
    {

        for (auto i = machineMap.begin(); i != machineMap.end(); i++)
        {
            if (i->second && !(tick % (i->second->speed + 1)))
            {
                i->second->Tick();
                i->second->Output();
            }
        }
        for (auto i = machineMap.begin(); i != machineMap.end(); i++)
        {
            if (i->second) i->second->TransferItems();
        }

        tick++;
        if (tick > 60) tick = 1;

        conveyorState++;
        if (conveyorState == 8) conveyorState = 0;

        std::this_thread::sleep_for(std::chrono::milliseconds(int(16)));

        //std::cout << std::endl;
    }
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
                invertRotation = true;
                break;

            case SDLK_1:
                if (selectedMachine == MachineType::CONVEYOR) selectedMachine = MachineType::NONE;
                else selectedMachine = MachineType::CONVEYOR;
                break;

            case SDLK_2:
                if (selectedMachine == MachineType::MINER) selectedMachine = MachineType::NONE;
                else selectedMachine = MachineType::MINER;
                break;
            
            case SDLK_3:
                if (selectedMachine == MachineType::FURNACE) selectedMachine = MachineType::NONE;
                else selectedMachine = MachineType::FURNACE;
                break;

            case SDLK_r:
                if (invertRotation) machineRotation--;
                else machineRotation++;
                if (machineRotation == 4) machineRotation = 0;
                if (machineRotation == 255) machineRotation = 3;
                break;

            case SDLK_q:
                SampleMachine();
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
                invertRotation = false;
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
        
        if (mouseButtonDown[0] && mouseButtonDown[1]) break;
        if (mouseButtonDown[0]) PlaceMachine();
        if (mouseButtonDown[1]) DeleteMachine();
        break;
    }
}

void Game::GenerateTerrain()
{
    const int chunkSize = 8;

    std::default_random_engine generator;
    std::uniform_int_distribution<int> type(1, 2);
    std::uniform_int_distribution<int> oreChunk(1, 80);
    std::uniform_int_distribution<int> voidChance(0, 1);

    TileType currentChunkOreType = TileType::NONE;

    for (int i = 0; i < 1000000; i++)
    {
        tiles[i].type = TileType::NONE;
    }

    for (int i = 1; i < 999 / chunkSize; i++)
    {
        for (int j = 1; j < 999 / chunkSize; j++)
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
                    if ((chunkJ == 0 || chunkJ == chunkSize - 1 || chunkI == 0 || chunkI == chunkSize - 1) && voidChance(generator) != 0) tiles[(i * chunkSize) + chunkI + ((j * chunkSize) + chunkJ) * 1000].type = TileType::NONE;
                    else tiles[(i * chunkSize) + chunkI + ((j * chunkSize) + chunkJ) * 1000].type = currentChunkOreType;
                    
                }
            }

            tiles[(i * chunkSize) + 0 + ((j * chunkSize) + chunkSize - 1) * 1000].type = TileType::NONE;
            tiles[(i * chunkSize) + 0 + ((j * chunkSize) + 0) * 1000].type = TileType::NONE;
            tiles[(i * chunkSize) + chunkSize - 1 + ((j * chunkSize) + chunkSize - 1) * 1000].type = TileType::NONE;
            tiles[(i * chunkSize) + chunkSize - 1 + ((j * chunkSize) + 0) * 1000].type = TileType::NONE;
        }
    }
}

}