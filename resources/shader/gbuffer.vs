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
uniform vec3 offsets[1000];

void main()
{
	vec3 offset = offsets[gl_InstanceID];
    vec4 pos = vec4(aPos.x + 3*offset.x, aPos.y + 3*offset.y, aPos.z + 3*offset.z, 1.0);
    gl_Position = projectionMat * viewMat * modelMat * pos;
	
    vec4 worldPos = modelMat * vec4(pos.xyz, 1.0f);
    FragPos = worldPos.xyz;
	
    TexCoords = aTex;

    mat3 normalMat = transpose(inverse(mat3(modelMat)));
    Normal = normalMat * aNorm;
}