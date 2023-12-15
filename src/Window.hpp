#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <unistd.h>
#include <glm/glm.hpp>
#include "Block.hpp"
#include <vector>

class Spearstake
{
public:
    Spearstake(const std::pair<int, int> &dimensions, const std::string &title, const std::string &icon, const int &targetFps = 60);
    ~Spearstake();

    void run();

private:
    void init();
    void update();
    void render();
    void clean();

    bool isRunning;
    GLFWwindow *window;
    std::pair<int, int> WINDOW_DIMENSIONS;
    std::string WINDOW_TITLE;
    std::string WINDOW_ICON;
    int TARGET_FPS;

    Block *block;

    glm::vec3 cameraPosition;
    float cameraYaw;
    float cameraPitch;

    GLuint programID;
};

#endif // WINDOW_H