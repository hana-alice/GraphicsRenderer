#version 330 core
layout (location = 0) in vec3 aPos;
out vec4 Color;
out vec3 Position;

uniform vec4 outColor;

layout (std140) uniform Matrices
{
    mat4 viewMat;
    mat4 projectionMat;
};
uniform mat4 modelMat;

void main()
{
    Position = vec3(modelMat *vec4(aPos,1.0));
    gl_Position = projectionMat * viewMat * vec4(Position,1.0);
    Color = outColor;
};