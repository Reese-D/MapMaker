#ifndef MATRIX_OPERATIONS_HEADER
#define MATRIX_OPERATIONS_HEADER

#include <stdio.h>
#include <string.h>
#include <GL/glew.h>

typedef GLfloat vec4[4];
typedef GLfloat vec3[3];
typedef GLfloat vec2[2];

typedef vec4 mat4[4];

void multVec4(vec4 altered, vec4 multiplier);
void mvMult(mat4 matrice, vec4 vector);

void translate(vec3 translation, GLfloat* out);
#endif
