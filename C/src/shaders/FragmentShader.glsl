#version 330 core

in vec3 colorTransfer;
//uniform vec3 triangleColor;
out vec4 outColor;

void main()
{
    outColor = vec4(colorTransfer, 1.0);
   // outColor = vec4(triangleColor, 1.0);
}
