/**
 * Copyright 2023 Gaspard Wierzbinski
 * @file Shaders.cpp
 * @brief Shader loader
 * @details This file contains the implementation of the shader loader
 */

#include "Shaders.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

/**
 * @brief Loads shaders from files
 * @param vertexFilePath Path to the vertex shader file
 * @param fragmentFilePath Path to the fragment shader file
 * @return The program ID
 */
GLuint LoadShaders(const char *vertexFilePath, const char *fragmentFilePath)
{
    // Create the shaders
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string vertexShaderCode;

    std::ifstream vertexShaderStream(vertexFilePath, std::ios::in);
    if (vertexShaderStream.is_open())
    {
        std::string line = "";
        while (getline(vertexShaderStream, line))
        {
            vertexShaderCode += "\n" + line;
        }
        vertexShaderStream.close();
    }
    else
    {
        std::cout << "Could not open " << vertexFilePath << std::endl;
        return 0;
    }

    // Read the Fragment Shader code from the file
    std::string fragmentShaderCode;
    std::ifstream fragmentShaderStream(fragmentFilePath, std::ios::in);
    if (fragmentShaderStream.is_open())
    {
        std::string line = "";
        while (getline(fragmentShaderStream, line))
        {
            fragmentShaderCode += "\n" + line;
        }
        fragmentShaderStream.close();
    }
    else
    {
        std::cout << "Could not open " << fragmentFilePath << std::endl;
        return 0;
    }

    GLint result = GL_FALSE;
    int infoLogLength;

    // Compile Vertex Shader
    std::cout << "Compiling shader: " << vertexFilePath << std::endl;
    char const *vertexSourcePointer = vertexShaderCode.c_str();
    glShaderSource(vertexShaderID, 1, &vertexSourcePointer, NULL);
    glCompileShader(vertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0)
    {
        std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
        glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
        std::cout << &vertexShaderErrorMessage[0] << std::endl;
    }

    // Compile Fragment Shader
    std::cout << "Compiling shader: " << fragmentFilePath << std::endl;
    char const *fragmentSourcePointer = fragmentShaderCode.c_str();
    glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer, NULL);
    glCompileShader(fragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0)
    {
        std::vector<char> fragmentShaderErrorMessage(infoLogLength + 1);
        glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
        std::cout << &fragmentShaderErrorMessage[0] << std::endl;
    }

    // Link the program
    std::cout << "Linking program" << std::endl;
    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    // Check the program
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0)
    {
        std::vector<char> programErrorMessage(infoLogLength + 1);
        glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
        std::cout << &programErrorMessage[0] << std::endl;
    }

    // Detach shaders
    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);

    // Delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    return programID;
}