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
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

Spearstake::Spearstake(const std::pair<int, int> &dimensions, const std::string &title, const std::string &icon, const int &targetFps)
    : isRunning(false), window(nullptr), WINDOW_DIMENSIONS(dimensions), WINDOW_TITLE(title), WINDOW_ICON(icon), TARGET_FPS(targetFps), cameraPosition(0.0f, 0.0f, 0.0f), cameraYaw(0.0f), cameraPitch(0.0f)
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

    glEnable(GL_DEPTH_TEST);

    // Set the viewport
    glViewport(0, 0, WINDOW_DIMENSIONS.first, WINDOW_DIMENSIONS.second);

    // Set the clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    isRunning = true;
}

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
    glClear(GL_COLOR_BUFFER_BIT);

    // Calculate the rotation matrix based on camera's yaw and pitch angles
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), cameraYaw, glm::vec3(0.0f, 1.0f, 0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, cameraPitch, glm::vec3(1.0f, 0.0f, 0.0f));

    // Calculate the view matrix by combining the rotation matrix and camera position
    glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), -cameraPosition) * rotationMatrix;

    // Render game objects here
    // block->render(viewMatrix);

    // Render yellow cube
    GLfloat vertices[] = {
        // Front face
        -0.5f, -0.5f, -0.5f, // Vertex 0
        0.5f, -0.5f, -0.5f,  // Vertex 1
        0.5f, 0.5f, -0.5f,   // Vertex 2
        -0.5f, -0.5f, -0.5f, // Vertex 0
        0.5f, 0.5f, -0.5f,   // Vertex 2
        -0.5f, 0.5f, -0.5f,  // Vertex 3

        // Back face
        -0.5f, -0.5f, 0.5f, // Vertex 4
        0.5f, -0.5f, 0.5f,  // Vertex 5
        0.5f, 0.5f, 0.5f,   // Vertex 6
        -0.5f, -0.5f, 0.5f, // Vertex 4
        0.5f, 0.5f, 0.5f,   // Vertex 6
        -0.5f, 0.5f, 0.5f,  // Vertex 7

        // Left face
        -0.5f, -0.5f, 0.5f,  // Vertex 8
        -0.5f, -0.5f, -0.5f, // Vertex 9
        -0.5f, 0.5f, -0.5f,  // Vertex 10
        -0.5f, -0.5f, 0.5f,  // Vertex 8
        -0.5f, 0.5f, -0.5f,  // Vertex 10
        -0.5f, 0.5f, 0.5f,   // Vertex 11

        // Right face
        0.5f, -0.5f, 0.5f,  // Vertex 12
        0.5f, -0.5f, -0.5f, // Vertex 13
        0.5f, 0.5f, -0.5f,  // Vertex 14
        0.5f, -0.5f, 0.5f,  // Vertex 12
        0.5f, 0.5f, -0.5f,  // Vertex 14
        0.5f, 0.5f, 0.5f,   // Vertex 15

        // Top face
        -0.5f, 0.5f, -0.5f, // Vertex 16
        0.5f, 0.5f, -0.5f,  // Vertex 17
        0.5f, 0.5f, 0.5f,   // Vertex 18
        -0.5f, 0.5f, -0.5f, // Vertex 16
        0.5f, 0.5f, 0.5f,   // Vertex 18
        -0.5f, 0.5f, 0.5f,  // Vertex 19

        // Bottom face
        -0.5f, -0.5f, -0.5f, // Vertex 20
        0.5f, -0.5f, -0.5f,  // Vertex 21
        0.5f, -0.5f, 0.5f,   // Vertex 22
        -0.5f, -0.5f, -0.5f, // Vertex 20
        0.5f, -0.5f, 0.5f,   // Vertex 22
        -0.5f, -0.5f, 0.5f   // Vertex 23
    };

    // Set the color to solid yellow
    GLfloat colors[] = {
        1.0f, 1.0f, 0.0f, // Vertex 0
        1.0f, 1.0f, 0.0f, // Vertex 1
        1.0f, 1.0f, 0.0f, // Vertex 2
        1.0f, 1.0f, 0.0f, // Vertex 3
        1.0f, 1.0f, 0.0f, // Vertex 4
        1.0f, 1.0f, 0.0f, // Vertex 5
        1.0f, 1.0f, 0.0f, // Vertex 6
        1.0f, 1.0f, 0.0f, // Vertex 7
        1.0f, 1.0f, 0.0f, // Vertex 8
        1.0f, 1.0f, 0.0f, // Vertex 9
        1.0f, 1.0f, 0.0f, // Vertex 10
        1.0f, 1.0f, 0.0f, // Vertex 11
        1.0f, 1.0f, 0.0f, // Vertex 12
        1.0f, 1.0f, 0.0f, // Vertex 13
        1.0f, 1.0f, 0.0f, // Vertex 14
        1.0f, 1.0f, 0.0f, // Vertex 15
        1.0f, 1.0f, 0.0f, // Vertex 16
        1.0f, 1.0f, 0.0f, // Vertex 17
        1.0f, 1.0f, 0.0f, // Vertex 18
        1.0f, 1.0f, 0.0f, // Vertex 19
        1.0f, 1.0f, 0.0f, // Vertex 20
        1.0f, 1.0f, 0.0f, // Vertex 21
        1.0f, 1.0f, 0.0f, // Vertex 22
        1.0f, 1.0f, 0.0f  // Vertex 23
    };

    GLuint indices[] = {
        0, 1, 2,    // Front face
        0, 2, 3,    // Front face
        4, 5, 6,    // Back face
        4, 6, 7,    // Back face
        8, 9, 10,   // Left face
        8, 10, 11,  // Left face
        12, 13, 14, // Right face
        12, 14, 15, // Right face
        16, 17, 18, // Top face
        16, 18, 19, // Top face
        20, 21, 22, // Bottom face
        20, 22, 23  // Bottom face
    };

    // Transform the vertices using the transformation matrix
    std::vector<GLfloat> transformedVertices;
    for (int i = 0; i < 24; i++)
    {
        glm::vec4 vertex(vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2], 1.0f);
        glm::vec4 transformedVertex = viewMatrix * vertex;
        transformedVertices.push_back(transformedVertex.x);
        transformedVertices.push_back(transformedVertex.y);
        transformedVertices.push_back(transformedVertex.z);
    }

    // Render the transformed cube
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, transformedVertices.data());
    glColorPointer(3, GL_FLOAT, 0, colors);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indices);
    glDisableClientState(GL_VERTEX_ARRAY);

    // Print rotation
    std::cout << "Camera yaw: " << cameraYaw << std::endl;

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Spearstake::clean()
{
    // Cleanup GLFW resources
    glfwTerminate();
}
