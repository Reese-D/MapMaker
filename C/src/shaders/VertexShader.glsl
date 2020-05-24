#version 330 core

in vec2 position;
in vec3 colorIn;

out vec3 colorTransfer;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    colorTransfer = colorIn;
    gl_Position = projection * view * model * vec4(position, 0.0, 1.0);
}
