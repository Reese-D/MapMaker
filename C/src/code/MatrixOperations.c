#include "MatrixOperations.h"


//Assumes that array has 3 values correlating to x, y, z
//stride lets us know how long the row is for each vertex point
//skip lets us know if there are any we need to ignore that are color or other info
//size lets us know the whole size of the array
void translate(vec3 translation, GLfloat* out){
    
    GLfloat translationMatrix[16] =
	{  1.0f, 0.0f, 0.0f, 0.0f,
	   0.0f, 1.0f, 0.0f, 0.0f,
	   0.0f, 0.0f, 1.0f, 0.0f,
	   0.2f, 0.2f, 0.0f, 1.0f };

    for(int i = 0; i < 16; i++){
	out[i] = translationMatrix[i];
    }
}


void multVec4(vec4 altered, vec4 multiplier){
    for(int i = 0; i < 4; i++){
	altered[i] = altered[i] * multiplier[i];
    }
}

void mvMult(mat4 matrice, vec4 vector){
    multVec4(vector, matrice[0]);
    multVec4(vector, matrice[1]);
    multVec4(vector, matrice[2]);
    multVec4(vector, matrice[3]);
}
