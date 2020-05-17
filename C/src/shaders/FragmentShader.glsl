#version 330 core

in vec3 colorTransfer;
out vec4 outColor;

void main()
{
    outColor = vec4(colorTransfer, 1.0);
}
