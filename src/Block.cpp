/**
 * @file Block.cpp
 * @brief 3D block class
 * @details This file contains the implementation of the Block class
 */

#include "Block.hpp"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <unistd.h>
#include <vector>
#include <glm/ext/matrix_transform.hpp>
#include "DDSLoader.hpp"

/**
 * @brief Constructor for Block
 * @param position The position of the block
 * @param texture The path to the texture of the block
 * @param programID The ID of the shader program
 * @details This constructor initializes the block with the given position and texture
 */
Block::Block(const Position &position, const char *texturePath, const GLuint programID) : position(position), programID(programID), isGenerated(false)
{
    // Check if texture exists at path, if not, print error message and exit
    if (access(texturePath, F_OK) == -1)
    {
        std::cerr << "Texture " << texturePath << " does not exist" << std::endl;
        exit(1);
    }

    // Set texture parameters
    this->texture = loadDDS(texturePath);

    // Validate texture
    if (this->texture == 0)
    {
        std::cerr << "Texture " << texturePath << " is invalid" << std::endl;
        exit(1);
    }

    // Load texture
    this->textureID = glGetUniformLocation(programID, "myTextureSampler");
}

void Block::generateGeometry()
{
    const float blockSize = 1.0f; // Set the size of a single block here

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

    // Subtract 1.0f from each V coordinate, because we are using DDS, which is inverted
    for (int i = 1; i < 36 * 2; i += 2)
    {
        texCoords[i] = 1.0f - texCoords[i];
    }

    // Copy each texture coordinate to this->texCoords
    for (int i = 0; i < 36 * 2; i++)
    {
        this->texCoords[i] = texCoords[i];
    }

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &texCoordBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->texCoords), this->texCoords, GL_STATIC_DRAW);

    isGenerated = true;
}

Block::~Block()
{
    // Delete buffers and clean up textures
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &texCoordBuffer);
    glDeleteTextures(1, &texture);
}

void Block::update()
{
    // Update block logic here
}

void Block::render(const glm::mat4 &mvpMatrix, const GLuint mvpMatrixID, const GLuint textures[])
{
    GLuint Texture = textures[0];

    // Get a handle for our "myTextureSampler" uniform
    GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

    glUseProgram(programID);

    glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &mvpMatrix[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture);
    // Set our "myTextureSampler" sampler to use Texture Unit 0
    glUniform1i(TextureID, 0);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(
        0,        // attribute. No particular reason for 0, but must match the layout in the shader.
        3,        // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0,        // stride
        (void *)0 // array buffer offset
    );

    // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
    glVertexAttribPointer(
        1,        // attribute. No particular reason for 1, but must match the layout in the shader.
        2,        // size : U+V => 2
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0,        // stride
        (void *)0 // array buffer offset
    );

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

Position Block::getPosition()
{
    return position;
}