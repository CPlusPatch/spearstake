#ifndef SHADERS_HPP
#define SHADERS_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

GLuint LoadShaders(const char *vertexFilePath, const char *fragmentFilePath);

#endif // SHADERS_HPP