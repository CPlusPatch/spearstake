/**
 * Copyright 2023 Gaspard Wierzbinski
 * @file Window.cpp
 * @brief Window renderer class
 * @details This file contains the class for the Spearstake window renderer
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <iostream>
#include <utility>

class Spearstake
{
public:
    Spearstake(const std::pair<int, int> &dimensions, const std::string &title, const std::string &icon, const int &targetFps = 60)
        : isRunning(false), window(nullptr), renderer(nullptr), WINDOW_DIMENSIONS(dimensions), WINDOW_TITLE(title), WINDOW_ICON(icon), TARGET_FPS(targetFps)
    {
    }

    ~Spearstake()
    {
    }

    void run()
    {
        init();

        while (isRunning)
        {
            update();
            render();
        }

        clean();
    }

private:
    void init()
    {
        // Initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
            isRunning = false;
            return;
        }

        // Create SDL window
        window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_DIMENSIONS.first, WINDOW_DIMENSIONS.second, SDL_WINDOW_SHOWN);
        if (window == nullptr)
        {
            std::cerr << "Failed to create SDL window: " << SDL_GetError() << std::endl;
            isRunning = false;
            return;
        }

        // Create SDL renderer
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == nullptr)
        {
            std::cerr << "Failed to create SDL renderer: " << SDL_GetError() << std::endl;
            isRunning = false;
            return;
        }

        isRunning = true;
    }

    void update()
    {
        // Update game logic here
    }

    void render()
    {
        // Calculate the time it takes to render a frame
        static Uint32 previousFrameTime = SDL_GetTicks();
        Uint32 currentFrameTime = SDL_GetTicks();
        Uint32 frameTime = currentFrameTime - previousFrameTime;
        previousFrameTime = currentFrameTime;

        // Limit the frame rate if necessary
        const Uint32 frameDelay = 1000 / TARGET_FPS;
        if (frameTime < frameDelay)
        {
            SDL_Delay(frameDelay - frameTime);
        }

        // Clear the renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render game objects here

        // Present the renderer
        SDL_RenderPresent(renderer);
    }

    void clean()
    {
        // Cleanup SDL resources
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    bool isRunning;
    SDL_Window *window;
    SDL_Renderer *renderer;
    std::pair<int, int> WINDOW_DIMENSIONS;
    std::string WINDOW_TITLE;
    std::string WINDOW_ICON;
    int TARGET_FPS;
};
