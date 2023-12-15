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
#include "DDSLoader.hpp"

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
    : isRunning(false), window(nullptr), WINDOW_DIMENSIONS(dimensions), WINDOW_TITLE(title), WINDOW_ICON(icon), TARGET_FPS(targetFps), cameraPosition(0.0f, 0.0f, 0.0f), cameraYaw(0.0f), cameraPitch(0.0f), cameraFov(45.0f)
{
    this->initialFov = cameraFov;
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

        update(frameTime);
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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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

    // Print OpenGL version
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    // Context
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glfwSetCursorPos(window, WINDOW_DIMENSIONS.first / 2, WINDOW_DIMENSIONS.second / 2);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Mouse scroll callback
    glfwSetScrollCallback(window, [](GLFWwindow *window, double xoffset, double yoffset)
                          {
        Spearstake *spearstake = (Spearstake *)glfwGetWindowUserPointer(window);
        spearstake->cameraFov -= yoffset;
        if (spearstake->cameraFov < 1.0f)
            spearstake->cameraFov = 1.0f;
        if (spearstake->cameraFov > 45.0f)
            spearstake->cameraFov = 45.0f; });

    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    programID = LoadShaders("./shaders/vertex.vert", "./shaders/fragment.frag");

    // Projection matrix
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), (float)WINDOW_DIMENSIONS.first / (float)WINDOW_DIMENSIONS.second, 0.1f, 100.0f);

    glm::vec3 direction(
        cos(cameraPitch) * sin(cameraYaw),
        sin(cameraPitch),
        cos(cameraPitch) * cos(cameraYaw));

    // Right vector
    glm::vec3 right = glm::vec3(
        sin(cameraYaw - 3.14f / 2.0f),
        0,
        cos(cameraYaw - 3.14f / 2.0f));

    // Up vector
    glm::vec3 up = glm::cross(right, direction);

    // View matrix
    glm::mat4 viewMatrix = glm::lookAt(cameraPosition, cameraPosition + direction, up);

    // Model matrix
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    // Model-view-projection matrix
    mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;

    mvpMatrixID = glGetUniformLocation(programID, "MVP");

    // Create blocks
    blocks.push_back(Block(Position(0.0f, 0.0f, 0.0f), wrapPath("textures/dirt.DDS").c_str(), programID));
    blocks.push_back(Block(Position(1.0f, 0.0f, 0.0f), wrapPath("textures/dirt.DDS").c_str(), programID));
    isRunning = true;
}

/**
 * @brief Updates the window
 * @details Listens for keyboard input and updates the window accordingly
 * @param deltaTime The time it took to render the last frame, in seconds
 */
void Spearstake::update(double deltaTime)
{
    float speed = 3.0f;
    float mouseSpeed = 1.0f;

    // Get mouse position
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    // Reset mouse position to center of screen so it cant escape
    glfwSetCursorPos(window, WINDOW_DIMENSIONS.first / 2, WINDOW_DIMENSIONS.second / 2);

    // Compute new orientation
    cameraYaw -= mouseSpeed * (float)deltaTime * float(WINDOW_DIMENSIONS.first / 2 - mouseX); // Invert yaw
    cameraPitch += mouseSpeed * (float)deltaTime * float(WINDOW_DIMENSIONS.second / 2 - mouseY);

    glm::vec3 direction(
        cos(cameraPitch) * sin(cameraYaw),
        sin(cameraPitch),
        cos(cameraPitch) * cos(cameraYaw));

    // Right vector
    glm::vec3 right = glm::vec3(
        sin(cameraYaw - 3.14f / 2.0f),
        0,
        cos(cameraYaw - 3.14f / 2.0f));

    // Up vector
    glm::vec3 up = glm::cross(right, direction);

    // Move forward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPosition += direction * (float)deltaTime * speed;
    }
    // Move backward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPosition -= direction * (float)deltaTime * speed;
    }
    // Strafe right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPosition += right * (float)deltaTime * speed;
    }
    // Strafe left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraPosition -= right * (float)deltaTime * speed;
    }

    // Compute matrices
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(cameraFov), (float)WINDOW_DIMENSIONS.first / (float)WINDOW_DIMENSIONS.second, 0.1f, 100.0f);

    glm::mat4 viewMatrix = glm::lookAt(
        cameraPosition,
        cameraPosition + direction,
        up);

    glm::mat4 modelMatrix = glm::mat4(1.0f);

    mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;

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
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLuint Texture = loadDDS("../textures/dirt.DDS");

    const GLuint textures[] = {Texture};

    // Render blocks
    for (Block &block : blocks)
    {
        if (!block.isGenerated)
            block.generateGeometry();
        block.render(mvpMatrix, mvpMatrixID, textures);
    }

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();

    // Clear all errors
    while (glGetError() != GL_NO_ERROR)
        ;
}

void Spearstake::clean()
{
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &vertexArrayID);
    // Cleanup GLFW resources
    glfwTerminate();
}
