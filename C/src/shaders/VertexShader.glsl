#version 330 core

in vec2 position;
in vec3 colorIn;
uniform mat4 trans;

out vec3 colorTransfer;


void main()
{
    colorTransfer = colorIn;
    gl_Position = trans * vec4(position, 0.0, 1.0);
}
