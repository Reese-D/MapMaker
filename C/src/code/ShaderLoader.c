#include "ShaderLoader.h"

GLuint loadShader(FILE* fp, GLenum shaderType, const char* buffer)
{
    if(fp == NULL){
	fprintf(stderr, "failed to load shader\n");
	exit(1);
    }
    
    size_t* len;
    fprintf(stderr, "%s", buffer);
    fprintf(stderr, "----------\n");
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &buffer, NULL);
    glCompileShader(shader);
    
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);


    char logBuffer[512];
    glGetShaderInfoLog(shader, 512, NULL, logBuffer);

    if(status != GL_TRUE){
	fprintf(stderr, "Fatal error occured when attempting to compile shader \n");
    }

    return shader;
}
