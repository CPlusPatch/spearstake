/**
 * @file Block.cpp
 * @brief 3D block class
 * @details This file contains the implementation of the Block class
 */

#include "Block.hpp"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <IL/il.h>
#include <IL/ilut.h>
#include <unistd.h>
#include <vector>
#include <glm/ext/matrix_transform.hpp>

/**
 * @brief Constructor for Block
 * @param position The position of the block
 * @param texture The path to the texture of the block
 * @param programID The ID of the shader program
 * @details This constructor initializes the block with the given position and texture
 */
Block::Block(const Position &position, const char *texture, const GLuint programID) : position(position), programID(programID)
{
    // Check if texture exists at path, if not, print error message and exit
    /* if (access(texture, F_OK) == -1)
    {
        std::cerr << "Texture " << texture << " does not exist" << std::endl;
        exit(1);
    } */

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load texture data into the texture object
    ILuint image;
    ilGenImages(1, &image);
    ilBindImage(image);
    ilLoadImage(texture);

    if (ilGetError() != IL_NO_ERROR)
    {
        std::cerr << "Error loading texture " << texture << std::endl;
        exit(1);
    }

    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    int width = ilGetInteger(IL_IMAGE_WIDTH);
    int height = ilGetInteger(IL_IMAGE_HEIGHT);

    if (width <= 0 || height <= 0)
    {
        std::cerr << "Invalid image dimensions for texture " << texture << std::endl;
        exit(1);
    }

    unsigned char *imageData = ilGetData();
    if (!imageData)
    {
        std::cerr << "Error getting image data for texture " << texture << std::endl;
        exit(1);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

    // Load texture
    GLuint textureID;
    glGenTextures(1, &textureID);

    // Bind the texture object
    glBindTexture(GL_TEXTURE_2D, textureID);

    this->textureID = glGetUniformLocation(programID, "myTextureSampler");

    // Validate OpenGL texture
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "Error loading texture " << texture << ": " << gluErrorString(error) << std::endl;
        exit(1);
    }

    // Generate mipmaps
    // glGenerateMipmap(GL_TEXTURE_2D);

    // Clean up DevIL resources
    ilDeleteImages(1, &image);

    float blockSize = 1.0f; // Set the size of a single block here

    GLfloat vertices[36 * 3] = {
        // Front face
        this->position.getX() - blockSize / 2, this->position.getY() - blockSize / 2, this->position.getZ() - blockSize / 2, // Vertex 0
        this->position.getX() + blockSize / 2, this->position.getY() - blockSize / 2, this->position.getZ() - blockSize / 2, // Vertex 1
        this->position.getX() + blockSize / 2, this->position.getY() + blockSize / 2, this->position.getZ() - blockSize / 2, // Vertex 2
        this->position.getX() - blockSize / 2, this->position.getY() - blockSize / 2, this->position.getZ() - blockSize / 2, // Vertex 0
        this->position.getX() + blockSize / 2, this->position.getY() + blockSize / 2, this->position.getZ() - blockSize / 2, // Vertex 2
        this->position.getX() - blockSize / 2, this->position.getY() + blockSize / 2, this->position.getZ() - blockSize / 2, // Vertex 3

        // Back face
        this->position.getX() - blockSize / 2, this->position.getY() - blockSize / 2, this->position.getZ() + blockSize / 2, // Vertex 4
        this->position.getX() + blockSize / 2, this->position.getY() - blockSize / 2, this->position.getZ() + blockSize / 2, // Vertex 5
        this->position.getX() + blockSize / 2, this->position.getY() + blockSize / 2, this->position.getZ() + blockSize / 2, // Vertex 6
        this->position.getX() - blockSize / 2, this->position.getY() - blockSize / 2, this->position.getZ() + blockSize / 2, // Vertex 4
        this->position.getX() + blockSize / 2, this->position.getY() + blockSize / 2, this->position.getZ() + blockSize / 2, // Vertex 6
        this->position.getX() - blockSize / 2, this->position.getY() + blockSize / 2, this->position.getZ() + blockSize / 2, // Vertex 7

        // Left face
        this->position.getX() - blockSize / 2, this->position.getY() - blockSize / 2, this->position.getZ() + blockSize / 2, // Vertex 8
        this->position.getX() - blockSize / 2, this->position.getY() - blockSize / 2, this->position.getZ() - blockSize / 2, // Vertex 9
        this->position.getX() - blockSize / 2, this->position.getY() + blockSize / 2, this->position.getZ() - blockSize / 2, // Vertex 10
        this->position.getX() - blockSize / 2, this->position.getY() - blockSize / 2, this->position.getZ() + blockSize / 2, // Vertex 8
        this->position.getX() - blockSize / 2, this->position.getY() + blockSize / 2, this->position.getZ() - blockSize / 2, // Vertex 10
        this->position.getX() - blockSize / 2, this->position.getY() + blockSize / 2, this->position.getZ() + blockSize / 2, // Vertex 11

        // Right face
        this->position.getX() + blockSize / 2, this->position.getY() - blockSize / 2, this->position.getZ() + blockSize / 2, // Vertex 12
        this->position.getX() + blockSize / 2, this->position.getY() - blockSize / 2, this->position.getZ() - blockSize / 2, // Vertex 13
        this->position.getX() + blockSize / 2, this->position.getY() + blockSize / 2, this->position.getZ() - blockSize / 2, // Vertex 14
        this->position.getX() + blockSize / 2, this->position.getY() - blockSize / 2, this->position.getZ() + blockSize / 2, // Vertex 12
        this->position.getX() + blockSize / 2, this->position.getY() + blockSize / 2, this->position.getZ() - blockSize / 2, // Vertex 14
        this->position.getX() + blockSize / 2, this->position.getY() + blockSize / 2, this->position.getZ() + blockSize / 2, // Vertex 15

        // Top face
        this->position.getX() - blockSize / 2, this->position.getY() + blockSize / 2, this->position.getZ() - blockSize / 2, // Vertex 16
        this->position.getX() + blockSize / 2, this->position.getY() + blockSize / 2, this->position.getZ() - blockSize / 2, // Vertex 17
        this->position.getX() + blockSize / 2, this->position.getY() + blockSize / 2, this->position.getZ() + blockSize / 2, // Vertex 18
        this->position.getX() - blockSize / 2, this->position.getY() + blockSize / 2, this->position.getZ() - blockSize / 2, // Vertex 16
        this->position.getX() + blockSize / 2, this->position.getY() + blockSize / 2, this->position.getZ() + blockSize / 2, // Vertex 18
        this->position.getX() - blockSize / 2, this->position.getY() + blockSize / 2, this->position.getZ() + blockSize / 2, // Vertex 19

        // Bottom face
        this->position.getX() - blockSize / 2, this->position.getY() - blockSize / 2, this->position.getZ() - blockSize / 2, // Vertex 20
        this->position.getX() + blockSize / 2, this->position.getY() - blockSize / 2, this->position.getZ() - blockSize / 2, // Vertex 21
        this->position.getX() + blockSize / 2, this->position.getY() - blockSize / 2, this->position.getZ() + blockSize / 2, // Vertex 22
        this->position.getX() - blockSize / 2, this->position.getY() - blockSize / 2, this->position.getZ() - blockSize / 2, // Vertex 20
        this->position.getX() + blockSize / 2, this->position.getY() - blockSize / 2, this->position.getZ() + blockSize / 2, // Vertex 22
        this->position.getX() - blockSize / 2, this->position.getY() - blockSize / 2, this->position.getZ() + blockSize / 2  // Vertex 23
    };

    // Copy each vertex to this->vertices
    for (int i = 0; i < 36 * 3; i++)
    {
        this->vertices[i] = vertices[i];
    }

    GLfloat texCoords[36 * 2] = {
        // Front face
        0.0f, 0.0f, // Vertex 0
        1.0f, 0.0f, // Vertex 1
        1.0f, 1.0f, // Vertex 2
        0.0f, 0.0f, // Vertex 0
        1.0f, 1.0f, // Vertex 2
        0.0f, 1.0f, // Vertex 3

        // Back face
        0.0f, 0.0f, // Vertex 4
        1.0f, 0.0f, // Vertex 5
        1.0f, 1.0f, // Vertex 6
        0.0f, 0.0f, // Vertex 4
        1.0f, 1.0f, // Vertex 6
        0.0f, 1.0f, // Vertex 7

        // Left face
        0.0f, 0.0f, // Vertex 8
        1.0f, 0.0f, // Vertex 9
        1.0f, 1.0f, // Vertex 10
        0.0f, 0.0f, // Vertex 8
        1.0f, 1.0f, // Vertex 10
        0.0f, 1.0f, // Vertex 11

        // Right face
        0.0f, 0.0f, // Vertex 12
        1.0f, 0.0f, // Vertex 13
        1.0f, 1.0f, // Vertex 14
        0.0f, 0.0f, // Vertex 12
        1.0f, 1.0f, // Vertex 14
        0.0f, 1.0f, // Vertex 15

        // Top face
        0.0f, 0.0f, // Vertex 16
        1.0f, 0.0f, // Vertex 17
        1.0f, 1.0f, // Vertex 18
        0.0f, 0.0f, // Vertex 16
        1.0f, 1.0f, // Vertex 18
        0.0f, 1.0f, // Vertex 19

        // Bottom face
        0.0f, 0.0f, // Vertex 20
        1.0f, 0.0f, // Vertex 21
        1.0f, 1.0f, // Vertex 22
        0.0f, 0.0f, // Vertex 20
        1.0f, 1.0f, // Vertex 22
        0.0f, 1.0f  // Vertex 23
    };

    // Copy each texture coordinate to this->texCoords
    for (int i = 0; i < 36 * 2; i++)
    {
        this->texCoords[i] = texCoords[i];
    }

    vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    texCoordBuffer;
    glGenBuffers(1, &texCoordBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
}

Block::~Block()
{
    glDeleteTextures(1, &textureID);
}

void Block::update()
{
    // Update block logic here
}

void Block::render(const glm::mat4 &mvpMatrix)
{

    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Set our "myTextureSampler" sampler to use Texture Unit 0
    glUniform1i(textureID, 0);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

    // Draw the cube
    glDrawArrays(GL_TRIANGLES, 0, 36); // 6 faces * 2 triangles per face * 3 vertices per triangle

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

Position Block::getPosition()
{
    return position;
}