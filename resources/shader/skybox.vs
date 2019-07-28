#version 330 core
layout (location = 0) in vec3 aPos;
out vec3 TexCoord;
uniform mat4 viewMat;
uniform mat4 projectionMat;

void main()
{
    vec4 pos = projectionMat * viewMat * vec4(aPos,1.0);
    gl_Position = pos.xyww;
    TexCoord = aPos;
};
