#version 330 core

in vec2 position;

in vec3 colorIn;
out vec3 colorTransfer;

void main()
{
    colorTransfer = colorIn;
    gl_Position = vec4(position, 0.0, 1.0);
}
