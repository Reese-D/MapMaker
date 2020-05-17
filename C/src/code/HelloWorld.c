#include <stdio.h>
#include <stdlib.h>

#include "HelloWorld.h"
#include "ShaderLoader.h"
#include "MatrixOperations.h"
#include "ReadFile.h"

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
    
    float vertices[] = {
    -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left
     0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
    -0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left
    };
    
    GLuint elements[] = {
    0, 1, 2,
    2, 3, 0
    };

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
    vec3 translationVector = {0.2f, 0.2f, 0.2f};
    GLfloat translation[16];

    translate(translationVector, translation);

    GLint uniTrans = glGetUniformLocation(shaderProgram, "trans");

    /* float* t = &translation[0]; */
    for(int i = 0; i < 16; i++){
    	fprintf(stderr, "%f ", translation[i]);
    	if(i%4 == 3){
    	    fprintf(stderr, "\n");
    	}
    }

    glUniformMatrix4fv(uniTrans, 1, GL_FALSE, translation);

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), 0);


    GLint triangleColor = glGetAttribLocation(shaderProgram, "colorIn");
    glEnableVertexAttribArray(triangleColor);
    glVertexAttribPointer(triangleColor, 3, GL_FLOAT, GL_FALSE,
                       5*sizeof(GLfloat), (void*)(2*sizeof(GLfloat)));

    
    do{
	// Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
	glClear( GL_COLOR_BUFFER_BIT );

	// Draw nothing
	draw();

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
