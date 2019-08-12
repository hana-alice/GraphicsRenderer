#version 330 core
layout (location = 0) in vec3 aPos;
in vec3 aNorm;
out vec3 Normal;
out vec3 Position;
layout (std140) uniform Matrices
{
    mat4 viewMat;
    mat4 projectionMat;
};
uniform mat4 modelMat;

void main()
{
    Normal = mat3(transpose(inverse(modelMat))) * aNorm;
    Position = vec3(modelMat *vec4(aPos,1.0));
    gl_Position = projectionMat * viewMat * vec4(Position,1.0);
};