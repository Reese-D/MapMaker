#ifndef SHADER_LOADER
#define SHADER_LOADER

#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>

GLuint loadShader(FILE* fp, GLenum shaderType, const char* buffer);

#endif
