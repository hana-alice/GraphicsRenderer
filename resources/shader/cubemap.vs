#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 WorldPos;

uniform mat4 viewMat;
uniform mat4 projectionMat;

void main()
{
    WorldPos = aPos;
    gl_Position =  projectionMat * viewMat * vec4(WorldPos, 1.0);
}