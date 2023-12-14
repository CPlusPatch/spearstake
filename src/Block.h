#ifndef BLOCK_H
#define BLOCK_H

#include "Position.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

class Block
{
public:
    Block(const Position &position, const char *texture);
    ~Block();

    void update();
    void render();
    Position getPosition();

private:
    Position position;
    GLuint textureID;
};

#endif // BLOCK_H