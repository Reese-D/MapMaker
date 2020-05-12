#include "../headers/ShaderLoader.h"

void loadShader(FILE* fp, GLenum shaderType)
{
    if(fp == NULL){
	fprintf(stderr, "failed to load shader\n");
	exit(1);
    }
    size_t* len;
    char* buffer = NULL;
    ssize_t bytes_read = getdelim( &buffer, len, '\0', fp);
    
    if ( bytes_read == -1) {
	fprintf(stderr, "shader file couldn't be read");
	exit(1);
    }

    const char* constBuffer = buffer;
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &constBuffer, NULL);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    char logBuffer[512];
    glGetShaderInfoLog(shader, 512, NULL, logBuffer);

    
    if(status != GL_TRUE){
	fprintf(stderr, "Fatal error occured when attempting to compile shader \n");
    }

    fprintf(stderr, logBuffer);
    
}
