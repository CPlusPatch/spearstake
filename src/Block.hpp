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
    Block(const Position &position, const char *texturePath, const GLuint programID);
    ~Block();

    void update();
    void render(const glm::mat4 &mvpMatrix, const GLuint mvpMatrixID, const GLuint textures[]);
    void generateGeometry();
    Position getPosition();

    bool isGenerated;

private:
    Position position;
    GLuint programID;
    GLuint textureID;
    GLuint texture;
    GLuint vertexBuffer;
    GLuint texCoordBuffer;
    GLuint indexBuffer;
    GLfloat vertices[8 * 3];
    GLint indices[36];
    GLfloat texCoords[36 * 2];
};

#endif // BLOCK_HPP