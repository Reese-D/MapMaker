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
void mouseInputHandler(GLFWwindow *window, double sensitivity, double edgePercentage);
void scrollHandler(GLFWwindow *window, double xoffset, double yoffset);
double getNoise(double xValue, double yValue);
#endif
