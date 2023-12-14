/**
 * @file Block.cpp
 * @brief 3D block class
 * @details This file contains the class for a 3D block
 */

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include "Position.cpp"

class Block
{
public:
    Block(const Position &position, const char *texture)
        : position(position)
    {
        // Load texture
        glGenTextures(1, &textureID);

        // Bind the texture object
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Load texture data into the texture object
        int width, height;
        unsigned char *image = SOIL_load_image(texture, &width, &height, 0, SOIL_LOAD_RGB);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

        // Generate mipmaps
        glGenerateMipmap(GL_TEXTURE_2D);

        // Use the texture in your rendering pipeline
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }

    ~Block()
    {
        glDeleteTextures(1, &textureID);
    }

    void update()
    {
    }

    void render()
    {
        // Calculate the size of a single block
        float blockSize = 100.0f; // Set the size of a single block here

        // Calculate the vertices of the cube based on the world position and block size
        GLfloat vertices[] = {
            position.getX() * blockSize, position.getY() * blockSize, position.getZ() * blockSize,                   // Front top left
            (position.getX() + 1) * blockSize, position.getY() * blockSize, position.getZ() * blockSize,             // Front top right
            (position.getX() + 1) * blockSize, (position.getY() + 1) * blockSize, position.getZ() * blockSize,       // Front bottom right
            position.getX() * blockSize, (position.getY() + 1) * blockSize, position.getZ() * blockSize,             // Front bottom left
            position.getX() * blockSize, position.getY() * blockSize, (position.getZ() + 1) * blockSize,             // Back top left
            (position.getX() + 1) * blockSize, position.getY() * blockSize, (position.getZ() + 1) * blockSize,       // Back top right
            (position.getX() + 1) * blockSize, (position.getY() + 1) * blockSize, (position.getZ() + 1) * blockSize, // Back bottom right
            position.getX() * blockSize, (position.getY() + 1) * blockSize, (position.getZ() + 1) * blockSize        // Back bottom left
        };

        // Render the cube
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, vertices);
        glDrawArrays(GL_QUADS, 0, 24);
        glDisableClientState(GL_VERTEX_ARRAY);
    }

    Position getPosition()
    {
        return position;
    }

private:
    Position position;
    GLuint textureID;
};