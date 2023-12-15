/**
 * Copyright 2023 Gaspard Wierzbinski
 * @file Window.cpp
 * @brief Window renderer class
 * @details This file contains the class for the Spearstake window renderer
 */

#include "Window.hpp"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <unistd.h>
#include "Block.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Shaders.hpp"

/**
 * @brief Wraps a path with the project root directory
 * @param path The path to wrap
 * @return The wrapped path
 */
std::string wrapPath(const std::string &path)
{
    return std::string(get_current_dir_name()) + "/../" + path;
}

/**
 * @brief Constructor for Spearstake
 * @param dimensions The dimensions of the window
 * @param title The title of the window
 * @param icon The path to the icon of the window
 * @param targetFps The target FPS of the window (default: 60)
 */
Spearstake::Spearstake(const std::pair<int, int> &dimensions, const std::string &title, const std::string &icon, const int &targetFps)
    : isRunning(false), window(nullptr), WINDOW_DIMENSIONS(dimensions), WINDOW_TITLE(title), WINDOW_ICON(icon), TARGET_FPS(targetFps), cameraPosition(4.0f, 3.0f, 3.0f), cameraYaw(0.0f), cameraPitch(0.0f)
{
}

Spearstake::~Spearstake()
{
    // Free all blocks
    for (Block &block : blocks)
    {
        block.~Block();
    }

    // Run cleanup
    clean();
}

/**
 * @brief Runs the window
 * @details Runs the window and renders it
 */
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

/**
 * @brief Initializes the window and OpenGL
 * @details Initializes GLFW and GLEW, and creates the window
 */
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
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // For Mac OS X
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
    glewExperimental = true; // Needed in core profile

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        isRunning = false;
        return;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Set the viewport
    glViewport(0, 0, WINDOW_DIMENSIONS.first, WINDOW_DIMENSIONS.second);
    glMatrixMode(GL_PROJECTION_MATRIX);
    glLoadIdentity();
    gluPerspective(45, WINDOW_DIMENSIONS.first / WINDOW_DIMENSIONS.second, 0.1f, 100.0f);

    // Set the clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    programID = LoadShaders("./shaders/vertex.vert", "./shaders/fragment.frag");

    // Projection matrix
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), (float)WINDOW_DIMENSIONS.first / (float)WINDOW_DIMENSIONS.second, 0.1f, 100.0f);

    // View matrix
    glm::mat4 viewMatrix = glm::lookAt(cameraPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0, 1.0f, 0.0f));

    // Model matrix
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    // Model-view-projection matrix
    mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;

    mvpMatrixID = glGetUniformLocation(programID, "MVP");

    // Create blocks
    blocks.push_back(Block(Position(0.0f, 0.0f, 0.0f), wrapPath("textures/dirt.dds").c_str(), programID));
    blocks.push_back(Block(Position(1.0f, 0.0f, 0.0f), wrapPath("textures/dirt.dds").c_str(), programID));
    isRunning = true;
}

/**
 * @brief Updates the window
 * @details Listens for keyboard input and updates the window accordingly
 */
void Spearstake::update()
{
    // Check for keyboard input
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        // Move camera forward
        cameraPosition += glm::vec3(0.0f, 0.0f, -0.1f);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        // Move camera backward
        cameraPosition += glm::vec3(0.0f, 0.0f, 0.1f);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        // Rotate camera left
        cameraYaw -= 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        // Rotate camera right
        cameraYaw += 1.0f;
    }

    // Handle escape key
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        // Close the window
        isRunning = false;
    }
}

/**
 * @brief Renders the window
 * @details Renders all elements on window using OpenGL
 */
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(programID);

    glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &mvpMatrix[0][0]);

    // Render blocks
    for (Block &block : blocks)
    {
        block.render(mvpMatrix);
    }

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Spearstake::clean()
{
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &vertexArrayID);
    // Cleanup GLFW resources
    glfwTerminate();
}
