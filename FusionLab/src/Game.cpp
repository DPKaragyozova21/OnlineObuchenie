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
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0, 1);

    for (int i = 0; i < 10000 * 10000; i++)
    {
        tiles[i].type = (TileType)distribution(generator);
    }
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

        RenderView();
        camera.Update();

        
        if (1000 / FPS + deltaTime < 0) deltaTime = 0;
        SDL_Delay(1000 / FPS + deltaTime);
    }
}

void Game::RenderView()
{
    static sdl::SpriteEnum sprite;
    const Bounds2F& camBounds = camera.GetView();
    
    if (camBounds.x == -1) return;

    sdlHandler.StartFrame();

    for (int i = -1; i <= camBounds.w; i++)
    {
        for (int j = -1; j <= camBounds.h; j++)
        {
            if ((int)((floor(camBounds.x) + i) + ((floor(camBounds.y) + j) * 10000)) >= 0 && (int)((floor(camBounds.x) + i) + ((floor(camBounds.y) + j) * 10000)) < 100000000)
            {
                switch (tiles[(int)((floor(camBounds.x) + i) + ((floor(camBounds.y) + j) * 1000))].type)
                {
                case TileType::NONE:
                    sprite = sdl::SpriteEnum::TILE_NONE;
                    break;

                case TileType::O2:
                    sprite = sdl::SpriteEnum::TILE_O2;
                }
                sdlHandler.RenderSprite(sprite, { int(int(camBounds.x * 100) % 100 * (-0.32 * camera.zoom)) + int(i * (32 * camera.zoom)), int(int(camBounds.y * 100) % 100 * (-0.32 * camera.zoom)) + (j * int(32 * camera.zoom))});
            }
        }
    }

    sdlHandler.EndFrame();
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
                /*switch (currentEvent->)
                {

                }*/
            break;
        }
        break;

    }
}

}