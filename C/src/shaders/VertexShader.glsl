#version 330 core

in vec3 positionIn;
in vec3 colorIn;
in vec3 normalIn;

out Vertex
{
 vec4 normal;
 vec4 color;
} vertex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(positionIn, 1.0);
    vertex.normal = vec4(normalIn, 1.0);
    vertex.color = vec4(colorIn, 1.0);
}
