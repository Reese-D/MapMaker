#ifndef HELLO_WORLD_HEADER
#define HELLO_WORLD_HEADER

#include <stdbool.h>
#include <stdio.h>
#include <GL/glew.h>    //libglew-dev
#include <GLFW/glfw3.h> //libglfw3-dev

void draw();
void setup();
bool initializeGlew();
bool initializeGlfw();

#endif
