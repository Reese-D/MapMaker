#include <stdio.h>
#include <stdlib.h>
#include <cglm/call.h>

#include "HelloWorld.h"
#include "ShaderLoader.h"
//#include "MatrixOperations.h"
#include "ReadFile.h"
#include "CubicNoise.h"

int main(){
    bool success = true;
    
    success |= initializeGlfw();
    setup();
        
    GLFWwindow* window = glfwCreateWindow(1024, 768, "Tutorial 01", NULL, NULL);
    glfwMakeContextCurrent(window);
    
    success |= initializeGlew();

    //vertex array object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);// Ensure we can capture the escape key being pressed below
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    GLuint* bufferObjects;
    GLuint elementBufferObject;
    GLuint vertexBufferObject;

    cubicNoiseConfig config = cubicNoiseConfig2D(2541512, 10, 42949672, 42949672);

    const int dimensions = 10;
    const int verticesSize = dimensions * dimensions * 6;
    const float increment = (1.0f / dimensions) * 2.0f;

    float yValue = 1.0f - (increment / 2.0f);
    float vertices[verticesSize];
    int counter = 0;
    for(int i = 0; i < dimensions; i++){
    	float xValue = -1.0f + (increment / 2.0f);
    	for(int k = 0; k < dimensions; k++){
	    float noiseValue = cubicNoiseSample2D(config, xValue * 10, yValue* 10);
    	    vertices[counter] = xValue;
    	    vertices[counter + 1] = yValue;
    	    vertices[counter + 2] = 0.0f;
	    vertices[counter + 3] = noiseValue;
	    vertices[counter + 4] = noiseValue;
    	    vertices[counter + 5] = noiseValue;//rand() / (float) RAND_MAX;
	    fprintf(stderr, "%f\t", noiseValue);
    	    /* fprintf(stderr, "x: %f, y: %f\t", vertices[counter], vertices[counter+1]); */
    	    xValue += increment;
	    counter+=6;
    	}
    	yValue -= increment;
    }
    fprintf(stderr, "\n");
    /* //we're looping but exlcuding the last column AND row */
    /* //we're going to connect a point to the point one colum to its right and one row below to make a trianglecolor */
    /* //we're also going to square it off by taking those other two points, and the point to below and to the right of our original point to make another triangle */
    int elementSize = (dimensions - 1) * (dimensions - 1) * 2 * 3;
    GLuint elements[elementSize];
    counter = 0;
    for(int i = 0; i < dimensions - 1; i++){
    	for(int k = 0; k < dimensions - 1; k++){
    	    int index = i * dimensions + k;
    	    //first triangle
	    elements[counter++] = index;
    	    elements[counter++] = index + 1;
	    elements[counter++] = index + dimensions;
    	    //second triangle
    	    elements[counter++] = index + dimensions;
    	    elements[counter++] = index + 1;
	    elements[counter++] = index + dimensions + 1;
	}
    }
    /* float vertices[] = { */
    /* 			-0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Top-left */
    /* 			0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // Top-right */
    /* 			-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f,  // Bottom-left */
    /* 			0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f // Bottom-right */
    /* }; */

    /* GLuint elements[] = { */
    /* 0, 1, 2, */
    /* 2, 3, 0 */
    /* }; */

    fprintf(stderr, "vertice count: %i\n", verticesSize);
    fprintf(stderr, "element count: %i\n", elementSize);
    for(int i = 0; i < elementSize; i+=3){
    	fprintf(stderr, "%i, %i, %i\n", elements[i], elements[i+1], elements[i+2]);
    }
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &elementBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    //load shaders
    FILE* fragment_fp = fopen("./src/shaders/FragmentShader.glsl", "r");
    char* fragmentBuffer = loadFile(fragment_fp);
    GLuint fragmentShader = loadShader(fragment_fp, GL_FRAGMENT_SHADER, fragmentBuffer);

    FILE* vertex_fp = fopen("./src/shaders/VertexShader.glsl", "r");
    char* vertexBuffer = loadFile(vertex_fp);
    GLuint vertexShader = loadShader(vertex_fp, GL_VERTEX_SHADER, vertexBuffer);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    //TRANSFORMS
    mat4 transformation = GLM_MAT4_IDENTITY_INIT;
    vec3 rotationAxis = {0.0f, 0.0f, 1.0f};

    //uses radians not degrees, clockwise is negative (45 degree clockwise rotation)
    glm_rotate(transformation, -M_PI/4.0f, rotationAxis);
    GLint uniTrans = glGetUniformLocation(shaderProgram, "model");

    glUniformMatrix4fv(uniTrans, 1, GL_FALSE, transformation[0]);

    vec3 eye = {1.2f, 1.2f, 1.2f};
    vec3 center = {0.0f, 0.0f, 0.0f};
    vec3 up = {0.0f, 0.0f, 1.0f};
    mat4 view;
    glm_lookat(eye, center, up, view);

    GLint uniView = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(uniView, 1, GL_FALSE, view[0]);

    //800 / 600 is the aspect ratio of the screen
    mat4 projection;
    glm_perspective(0.785398f, 800.0f / 600.0f, 1.0f, 10.0f, projection);
    GLint uniProjection = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(uniProjection, 1, GL_FALSE, projection[0]);

    
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), 0);


    GLint triangleColor = glGetAttribLocation(shaderProgram, "colorIn");
    glEnableVertexAttribArray(triangleColor);
    glVertexAttribPointer(triangleColor, 3, GL_FLOAT, GL_FALSE,
                       6*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));


    do{
	// Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
	glClear( GL_COLOR_BUFFER_BIT );

	//draw();
	glDrawElements(GL_TRIANGLES, elementSize, GL_UNSIGNED_INT, 0);
	
	// Swap buffers
	glfwSwapBuffers(window);
	glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
	   glfwWindowShouldClose(window) == 0 );

    // Close OpenGL window and terminate GLFW
    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteBuffers(1, &vertexBufferObject);
    glDeleteBuffers(1, &elementBufferObject);
    glDeleteVertexArrays(1, &vao);

    free(fragmentBuffer);
    free(vertexBuffer);
    glfwTerminate();

    return success;
}

void draw(){
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);	
}


void setup(){
    glfwWindowHint(GLFW_SAMPLES, 4); //4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 
}

bool initializeGlew(){
    glewExperimental = true;
    if(glewInit()){
	fprintf(stderr, "Failed to intialize GLEW\n");
	return false;
    }
    return true;
}

bool initializeGlfw(){
    if(!glfwInit()){
	fprintf(stderr, "Failed to initialize GLFW\n");
	return false;
    }
    return true;
}
