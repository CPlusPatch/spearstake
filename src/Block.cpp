/**
 * @file Block.cpp
 * @brief 3D block class
 * @details This file contains the implementation of the Block class
 */

#include "Block.hpp"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <unistd.h>
#include <vector>
#include <glm/ext/matrix_transform.hpp>

/**
 * @brief Constructor for Block
 * @param position The position of the block
 * @param texture The path to the texture of the block
 * @details This constructor initializes the block with the given position and texture
 */
Block::Block(const Position &position, const char *texture)
    : position(position)
{
    // Check if texture exists at path, if not, print error message and exit
    /* if (access(texture, F_OK) == -1)
    {
        std::cerr << "Texture " << texture << " does not exist" << std::endl;
        exit(1);
    }

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
    // glGenerateMipmap(GL_TEXTURE_2D);

    // Use the texture in your rendering pipeline
    // glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID); */
}

Block::~Block()
{
    glDeleteTextures(1, &textureID);
}

void Block::update()
{
    // Update block logic here
}

void Block::render(const glm::mat4 &viewMatrix)
{
    // Calculate the size of a single block
    float blockSize = 100.0f; // Set the size of a single block here

    // Calculate the model matrix for the block
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position.toVec3() * blockSize);

    // Combine the model matrix and the view matrix to get the final transformation matrix
    glm::mat4 transformationMatrix = viewMatrix * modelMatrix;

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

    // Set the color to solid yellow
    GLfloat colors[] = {
        1.0f, 1.0f, 0.0f, // Front top left
        1.0f, 1.0f, 0.0f, // Front top right
        1.0f, 1.0f, 0.0f, // Front bottom right
        1.0f, 1.0f, 0.0f, // Front bottom left
        1.0f, 1.0f, 0.0f, // Back top left
        1.0f, 1.0f, 0.0f, // Back top right
        1.0f, 1.0f, 0.0f, // Back bottom right
        1.0f, 1.0f, 0.0f  // Back bottom left
    };

    // Transform the vertices using the transformation matrix
    std::vector<GLfloat> transformedVertices;
    for (int i = 0; i < 24; i++)
    {
        glm::vec4 vertex(vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2], 1.0f);
        glm::vec4 transformedVertex = transformationMatrix * vertex;
        transformedVertices.push_back(transformedVertex.x);
        transformedVertices.push_back(transformedVertex.y);
        transformedVertices.push_back(transformedVertex.z);
    }

    // Render the transformed cube
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, transformedVertices.data());
    glColorPointer(3, GL_FLOAT, 0, colors);
    glDrawArrays(GL_QUADS, 0, 24);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}

Position Block::getPosition()
{
    return position;
}