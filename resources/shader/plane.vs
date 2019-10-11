#version 330 core
layout (location = 0) in vec3 aPos;
out vec2 TexCoord;
out vec4 lsPos;
in  vec2 aTex;

uniform mat4 lightSpaceMatrix;
uniform mat4 modelMat;

layout (std140) uniform Matrices
{
    mat4 viewMat;
    mat4 projectionMat;
};

void main()
{
    vec4 pos = projectionMat * viewMat * modelMat* vec4(aPos,1.0);
    gl_Position = pos;
    TexCoord = aTex;
    lsPos = lightSpaceMatrix * modelMat* vec4(aPos, 1.0);
};
