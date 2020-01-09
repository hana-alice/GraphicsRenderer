#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTex;
out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

layout (std140) uniform Matrices
{
    mat4 viewMat;
    mat4 projectionMat;
};

uniform mat4 modelMat;

void main()
{
    vec4 viewPos = viewMat * modelMat * vec4(aPos, 1.0f);
    FragPos = viewPos.xyz;
    gl_Position = projectionMat * viewPos;
	
    TexCoords = aTex;

    mat3 normalMat = transpose(inverse(mat3(viewMat * modelMat)));
    Normal = normalMat * aNorm;
}
