#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;
uniform mat4 model;

uniform vec3 offsets[1000];

layout (std140) uniform Matrices
{
    mat4 viewMat;
    mat4 projectionMat;
};

void main()
{
    vec3 offset = offsets[gl_InstanceID];
    vec4 pos = vec4(aPos.x + 3*offset.x, aPos.y + 3*offset.y, aPos.z + 3*offset.z, 1.0);
    gl_Position = projectionMat * viewMat * model * pos;
    FragPos = vec3(model*pos);
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTex;
}