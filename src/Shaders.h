#ifndef SHADERS_H
#define SHADERS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

GLuint LoadShaders(const char *vertexFilePath, const char *fragmentFilePath);

#endif // SHADERS_H