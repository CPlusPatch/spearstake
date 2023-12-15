#ifndef BLOCK_H
#define BLOCK_H

#include "Position.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>

class Block
{
public:
    Block(const Position &position, const char *texture);
    ~Block();

    void update();
    void render(const glm::mat4 &viewMatrix);
    Position getPosition();

private:
    Position position;
    GLuint textureID;
};

#endif // BLOCK_H