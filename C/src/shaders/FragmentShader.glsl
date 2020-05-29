#version 330 core

in vec4 vertex_color;
out vec4 outColor;

void main()
{
    outColor = vertex_color;
}

