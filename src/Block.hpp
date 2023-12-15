#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "Position.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>

class Block
{
public:
    Block(const Position &position, const char *texture, const GLuint programID);
    ~Block();

    void update();
    void render(const glm::mat4 &mvpMatrix);
    Position getPosition();

private:
    Position position;
    GLuint programID;
    GLuint textureID;
    GLuint texture;
    GLuint vertexBuffer;
    GLuint texCoordBuffer;
    GLfloat vertices[36 * 3];
    GLfloat texCoords[36 * 2];
};

#endif // BLOCK_HPP