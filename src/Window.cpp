/**
 * Copyright 2023 Gaspard Wierzbinski
 * @file Window.cpp
 * @brief Window renderer class
 * @details This file contains the class for the Spearstake window renderer
 */

#include "Window.h"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <unistd.h>
#include "Block.h"
#include <vector>

Spearstake::Spearstake(const std::pair<int, int> &dimensions, const std::string &title, const std::string &icon, const int &targetFps)
    : isRunning(false), window(nullptr), WINDOW_DIMENSIONS(dimensions), WINDOW_TITLE(title), WINDOW_ICON(icon), TARGET_FPS(targetFps)
{
    // Create blocks
    float blockDistance = 20.0f; // Distance of the block from the camera
    block = new Block(Position(0.0f, 0.0f, -blockDistance), "../textures/dirt.png");
}

Spearstake::~Spearstake()
{
    delete block; // Free the allocated memory for the Block object
}

void Spearstake::run()
{
    init();

    while (isRunning)
    {
        update();
        render();
    }

    clean();
}

void Spearstake::init()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        isRunning = false;
        return;
    }

    std::cout << "Initializing window" << std::endl;

    // Create GLFW window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(WINDOW_DIMENSIONS.first, WINDOW_DIMENSIONS.second, WINDOW_TITLE.c_str(), nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        isRunning = false;
        return;
    }

    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        isRunning = false;
        return;
    }

    isRunning = true;
}

void Spearstake::update()
{
    // Update game logic here
}

void Spearstake::render()
{
    // Calculate the time it takes to render a frame
    static double previousFrameTime = glfwGetTime();
    double currentFrameTime = glfwGetTime();
    double frameTime = currentFrameTime - previousFrameTime;
    previousFrameTime = currentFrameTime;

    // Limit the frame rate if necessary
    const double frameDelay = 1.0 / TARGET_FPS;
    if (frameTime < frameDelay)
    {
        double sleepTime = frameDelay - frameTime;
        usleep(sleepTime * 1000000);
    }

    // Clear the screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render game objects here
    block->render();

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Spearstake::clean()
{
    // Cleanup GLFW resources
    glfwTerminate();
}
