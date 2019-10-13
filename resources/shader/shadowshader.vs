#version 330 core
layout (location = 0) in vec3 position;

layout (std140) uniform Matrices
{
    mat4 viewMat;
    mat4 projectionMat;
};
uniform mat4 model;

void main()
{
    gl_Position = projectionMat * viewMat * model * vec4(position, 1.0f);
}