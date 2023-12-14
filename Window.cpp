/**
 * Copyright 2023 Gaspard Wierzbinski
 * @file Window.cpp
 * @brief Window renderer class
 * @details This file contains the class for the Spearstake window renderer
 */

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <unistd.h>

class Spearstake
{
public:
    Spearstake(const std::pair<int, int> &dimensions, const std::string &title, const std::string &icon, const int &targetFps = 60)
        : isRunning(false), window(nullptr), WINDOW_DIMENSIONS(dimensions), WINDOW_TITLE(title), WINDOW_ICON(icon), TARGET_FPS(targetFps)
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
        // Initialize GLFW
        if (!glfwInit())
        {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            isRunning = false;
            return;
        }

        // Create GLFW window
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    void update()
    {
        // Update game logic here
    }

    void render()
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

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    void clean()
    {
        // Cleanup GLFW resources
        glfwTerminate();
    }

    bool isRunning;
    GLFWwindow *window;
    std::pair<int, int> WINDOW_DIMENSIONS;
    std::string WINDOW_TITLE;
    std::string WINDOW_ICON;
    int TARGET_FPS;
};