#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>    //libglew-dev
#include <GLFW/glfw3.h> //libglfw3-dev


#include "HelloWorld.h"


int main(){
    bool success = true;
    success |= initializeGlfw();
    
    return -1;
}


bool initializeGlfw(){
    glewExperimental = true;
    if(!glfwInit()){
	fprintf(stderr, "Failed to initialize GLFW\n");
	return false;
    }
    return true;
}
