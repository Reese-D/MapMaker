#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <cglm/call.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

#include "HelloWorld.h"
#include "ShaderLoader.h"
//#include "MatrixOperations.h"
#include "ReadFile.h"
#include "open-simplex-noise.h"

#define dimensions 1024
#define windowWidth 1440
#define windowHeight 1440

#define threadCount 8 //max 16 right now

#define x_val 0
#define y_val 1
#define z_val 2
#define red 3
#define green 4
#define blue 5

typedef float vertices_t[dimensions][dimensions][6];
const int verticesSize = dimensions * dimensions * 6;
float *vertices;
const float increment = (1.0f / dimensions);

static mat4 transformation = GLM_MAT4_IDENTITY_INIT;
struct osn_context *ctx;

const double waterLevel = 0.4;
double noise_squish;

pthread_t threads[threadCount];
pthread_cond_t conditions[threadCount] = { PTHREAD_COND_INITIALIZER };
pthread_mutex_t locks[threadCount] = { PTHREAD_MUTEX_INITIALIZER };

static bool terminateThreads = false;

static uint64_t workStatus = 0;
const uint64_t workComplete = pow(2,threadCount) - 1;

static double totalZoom = 0.0;

pthread_mutex_t workMutex;

//assumes little endian
void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i=size-1;i>=0;i--)
    {
        for (j=7;j>=0;j--)
        {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}

void *threadFunc(void *voidargp){
    static int count;
    
    pthread_mutex_lock(&workMutex);
    int index = count++;
    pthread_mutex_unlock(&workMutex);
    
    //fprintf(stderr, "thread: %i created\n", index);
    while(!terminateThreads){
	pthread_cond_wait(&conditions[index], &locks[index]);
	//do work here
	if(terminateThreads){
	    break;
	}

	//fprintf(stderr, "thread %i starting work...\n", index);
	zoom(index);
	//set completion status
	pthread_mutex_lock(&workMutex);
	workStatus |= 1 << index;
	pthread_mutex_unlock(&workMutex);
    }
    //fprintf(stderr, "terminating thread %i\n", index);

    return NULL;
}

double getNoise(double xValue, double yValue){
    double v0, v1, v2;    
    
     /* Use three octaves: frequency N, N/2 and N/4 with relative amplitudes 4:2:1. */
    v0 = open_simplex_noise2(ctx, (double) xValue * noise_squish / 4.0,
			     (double) yValue * noise_squish  / 4.0);
    v1 = open_simplex_noise2(ctx, (double) xValue * noise_squish / 2.0,
			     (double) yValue * noise_squish  / 2);
    v2 = open_simplex_noise2(ctx, (double) xValue * noise_squish / 1.0,
			     (double) yValue * noise_squish  / 1);
    double noiseValue = v0 * 4.0 / 7.0 + v1 * 2.0 / 7.0 + v2 * 1.0 / 7.0;

    noiseValue += 0.5;
    if(noiseValue <= waterLevel){
	noiseValue = 0.0f;
    }

     return noiseValue;
}

int main(){
    srand(time(NULL));

    for(int i = 0; i < threadCount; i++){
	pthread_create(&(threads[i]), NULL, threadFunc, NULL);
    }

    //somewhere between 0 and 16 is probably good
    //noise_squish = (rand() / (double) RAND_MAX) * 16.0;
    noise_squish = 12.0;
    open_simplex_noise(105421, &ctx);
    
    bool success = true;
    
    success |= initializeGlfw();
    setup();
        
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Tutorial 01", NULL, NULL);
    glfwMakeContextCurrent(window);
    
    success |= initializeGlew();

    //vertex array object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);// Ensure we can capture the escape key being pressed below
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    GLuint elementBufferObject;
    GLuint vertexBufferObject;

    double rgb;
    float yValue = 0.5f;

    vertices = malloc(sizeof(double) * verticesSize);
    int counter = 0;

    for(int i = 0; i < dimensions; i++){
    	float xValue = -0.5f;
    	for(int k = 0; k < dimensions; k++){
	    if(k == 0 && i == 0){
		//fprintf(stderr, "%f,%f noise: %f\n",xValue, yValue, getNoise(xValue, yValue));
	    }
	    rgb = getNoise(xValue, yValue);
	    //fprintf(stderr, "%f\t%f\t%f\n",rgb, getNoise(xValue, yValue), getNoise(xValue, yValue));

    	    vertices[counter] = xValue;
    	    vertices[counter + 1] = yValue;
    	    vertices[counter + 2] = 0.0f;// rgb / height_squish;

	    vertices[counter + 3] = rgb;
	    vertices[counter + 4] = rgb;
    	    vertices[counter + 5] = rgb;

	    if(rgb == 0.0){
		vertices[counter + 5] = (1.0 - waterLevel);
		//vertices[counter + 2] = waterLevel / height_squish;
	    }
    	    xValue += increment;
	    counter+=6;
    	}
    	yValue -= increment;
    }
    /* fprintf(stderr, "min: %f\n", min); */
    /* fprintf(stderr, "max: %f\n", max); */
    //    fprintf(stderr, "%f\n", ((transform_t*)vertices)[3][3][1]);

    
    //we're looping but exlcuding the last column AND row
    //we're going to connect a point to the point one colum to its right and one row below to make a trianglecolor
    //we're also going to square it off by taking those other two points, and the point to below and to the right of our original point to make another triangle
    int elementSize = (dimensions - 1) * (dimensions - 1) * 2 * 3;
    GLuint *elements = malloc(sizeof(GLuint) * elementSize);
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

    //fprintf(stderr, "vertice count: %i\n", verticesSize);
    //fprintf(stderr, "element count: %i\n", elementSize);

    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesSize, vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &elementBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * elementSize, elements, GL_STATIC_DRAW);

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


    //uses radians not degrees, clockwise is negative (45 degree clockwise rotation)
    //vec3 rotationAxis = {0.0f, 0.0f, 1.0f};
    //glm_rotate(transformation, -M_PI/4.0f, rotationAxis);
    glm_perspective_resize(windowHeight * 1.0 / windowWidth, transformation);
    GLint uniTrans = glGetUniformLocation(shaderProgram, "model");

    glUniformMatrix4fv(uniTrans, 1, GL_FALSE, transformation[0]);

    vec3 eye = {0.0f, 0.0f, -1.2f};
    vec3 center = {0.0f, 0.0f, 0.0f};
    vec3 up = {0.0f, -1.0f, 0.0f};
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


    glfwSetScrollCallback(window, scrollHandler);
    do{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	mouseInputHandler(window, 0.03, 0.05);

	glUniformMatrix4fv(uniTrans, 1, GL_FALSE, transformation[0]);
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
    free(elements);
    free(vertices);
    open_simplex_noise_free(ctx);
    glfwTerminate();

    terminateThreads = true;
    for(int i = 0; i < threadCount; i++){
	//fprintf(stderr, "signaling %i\n", i);
	pthread_cond_signal(&conditions[i]);//signal one more time to make sure they hit the conditional loop again
    }

    for(int i = 0; i < threadCount; i++){
	pthread_join(threads[i], NULL);
    }


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
    glEnable(GL_DEPTH_TEST);
}

bool initializeGlew(){
    glewExperimental = true;
    if(glewInit()){
	//fprintf(stderr, "Failed to intialize GLEW\n");
	return false;
    }
    return true;
}

bool initializeGlfw(){
    if(!glfwInit()){
	//fprintf(stderr, "Failed to initialize GLFW\n");
	return false;
    }
    return true;
}

void mouseInputHandler(GLFWwindow* window, double sensitivity, double edgePercentage){
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    //make sure we aren't outside the window
    if(xpos < 0.0 || xpos > windowWidth || ypos < 0.0 || ypos > windowHeight){
	return;
    }
    if(xpos < windowWidth * edgePercentage){
    	glm_translate_x(transformation, sensitivity * -1.0);
    }
    else if(xpos > windowWidth - windowWidth * edgePercentage){
    	glm_translate_x(transformation, sensitivity);
    }

    if(ypos < windowHeight * edgePercentage){
    	glm_translate_y(transformation, sensitivity);
    }
    else if(ypos > windowHeight - windowHeight * edgePercentage){
    	glm_translate_y(transformation, sensitivity * -1.0);
    }
}


void scrollHandler(GLFWwindow *window, double xoffset, double yoffset){
    //this will change as we zoom in and out, might use it later for non-linear scaling
    //fprintfp(stderr, "%f\n", transformation[3][2]);
    totalZoom += (yoffset * 0.03);
    for(int i = 0; i < threadCount; i++){
	pthread_cond_signal(&conditions[i]);
    }

    while(workStatus != workComplete){
	//do nothing spinlock while threads finish up
    }
    
    pthread_mutex_lock(&workMutex);
    workStatus = 0;
    pthread_mutex_unlock(&workMutex);
    
    //fprintf(stderr, "finished\n");
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesSize, vertices, GL_STATIC_DRAW);
}

void zoom(int index){
    double result;
    
    int start = (dimensions / threadCount) * index;
    int end = (dimensions / threadCount) * (index + 1);

    for(int i = start; i < end; i++){
	for(int k = 0; k < dimensions; k++){
	    float x = (*(vertices_t*)vertices)[i][k][x_val];
	    float y = (*(vertices_t*)vertices)[i][k][y_val];

	    x *= 1.0 - totalZoom;
	    y *= 1.0 - totalZoom;
	    result = getNoise(x, y);

	    (*(vertices_t*)vertices)[i][k][red] = result;
	    (*(vertices_t*)vertices)[i][k][blue] = result;
	    (*(vertices_t*)vertices)[i][k][green] = result;

	    if(result == 0.0){
		(*(vertices_t*)vertices)[i][k][blue] = (1.0 - waterLevel);
	    }
	}
    }
}













