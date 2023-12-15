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
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Shaders.hpp"

Spearstake::Spearstake(const std::pair<int, int> &dimensions, const std::string &title, const std::string &icon, const int &targetFps)
    : isRunning(false), window(nullptr), WINDOW_DIMENSIONS(dimensions), WINDOW_TITLE(title), WINDOW_ICON(icon), TARGET_FPS(targetFps), cameraPosition(4.0f, 3.0f, 3.0f), cameraYaw(0.0f), cameraPitch(0.0f)
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

    // Set the viewport
    glViewport(0, 0, WINDOW_DIMENSIONS.first, WINDOW_DIMENSIONS.second);
    glMatrixMode(GL_PROJECTION_MATRIX);
    glLoadIdentity();
    gluPerspective(45, WINDOW_DIMENSIONS.first / WINDOW_DIMENSIONS.second, 0.1f, 100.0f);

    // Set the clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    programID = LoadShaders("./shaders/vertex.vert", "./shaders/fragment.frag");
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
}

/**
 * @brief Renders the window
 * @details Renders all elements on window using OpenGL
 */
void Spearstake::render()
{
    glUseProgram(programID);

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

    // Projection matrix
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), (float)WINDOW_DIMENSIONS.first / (float)WINDOW_DIMENSIONS.second, 0.1f, 100.0f);

    // View matrix
    glm::mat4 viewMatrix = glm::lookAt(cameraPosition, cameraPosition + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0, 1.0f, 0.0f));

    // Model matrix
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    // Model-view-projection matrix
    glm::mat4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;

    GLuint mvpMatrixID = glGetUniformLocation(programID, "MVP");
    glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &mvpMatrix[0][0]);

    // Draw a triangle
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f, // Vertex 1
        1.0f, -1.0f, 0.0f,  // Vertex 2
        0.0f, 1.0f, 0.0f    // Vertex 3
    };

    GLuint vertexBuffer;

    // Generate a buffer
    glGenBuffers(1, &vertexBuffer);

    // Bind the buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    // Send the data
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    // Draw the triangle
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
    /* // Render yellow cube
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

// Random colors
GLfloat colors[] = {
    1.0f, 0.0f, 0.0f, // Front face
    0.0f, 1.0f, 0.0f, // Front face
    0.0f, 0.0f, 1.0f, // Front face
    1.0f, 0.0f, 0.0f, // Front face
    0.0f, 0.0f, 1.0f, // Front face
    1.0f, 0.0f, 1.0f, // Front face

    1.0f, 0.0f, 0.0f, // Back face
    0.0f, 1.0f, 0.0f, // Back face
    0.0f, 0.0f, 1.0f, // Back face
    1.0f, 0.0f, 0.0f, // Back face
    0.0f, 0.0f, 1.0f, // Back face
    1.0f, 0.0f, 1.0f, // Back face

    1.0f, 0.0f, 0.0f, // Left face
    0.0f, 1.0f, 0.0f, // Left face
    0.0f, 0.0f, 1.0f, // Left face
    1.0f, 0.0f, 0.0f, // Left face
    0.0f, 0.0f, 1.0f, // Left face
    1.0f, 0.0f, 1.0f, // Left face

    1.0f, 0.0f, 0.0f, // Right face
    0.0f, 1.0f, 0.0f, // Right face
    0.0f, 0.0f, 1.0f, // Right face
    1.0f, 0.0f, 0.0f, // Right face
    0.0f, 0.0f, 1.0f, // Right face
    1.0f, 0.0f, 1.0f, // Right face

    1.0f, 0.0f, 0.0f, // Top face
    0.0f, 1.0f, 0.0f, // Top face
    0.0f, 0.0f, 1.0f, // Top face
    1.0f, 0.0f, 0.0f, // Top face
    0.0f, 0.0f, 1.0f, // Top face
    1.0f, 0.0f, 1.0f, // Top face

    1.0f, 0.0f, 0.0f, // Bottom face
    0.0f, 1.0f, 0.0f, // Bottom face
    0.0f, 0.0f, 1.0f, // Bottom face
    1.0f, 0.0f, 0.0f, // Bottom face
    0.0f, 0.0f, 1.0f, // Bottom face
    1.0f, 0.0f, 1.0f  // Bottom face
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
*/
    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Spearstake::clean()
{
    // Cleanup GLFW resources
    glfwTerminate();
}
