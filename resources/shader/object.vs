#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;
uniform mat4 model;
layout (std140) uniform Matrices
{
    mat4 viewMat;
    mat4 projectionMat;
};

void main()
{
    gl_Position = projectionMat * viewMat * model * vec4(aPos, 1.0);
    FragPos = vec3(model*vec4(aPos,1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTex;
}