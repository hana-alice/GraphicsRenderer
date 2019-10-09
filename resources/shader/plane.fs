#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
in vec4 lsPos;

uniform sampler2D smp;
uniform sampler2D shadowSmp;
void main()
{
    FragColor = texture(smp, TexCoord);

    vec3 projCoords = lsPos.xyz/lsPos.z;
    projCoords = projCoords*0.5 + 0.5;
    float closestDepth = texture(shadowSmp,lsPos.xy).r;
    float currentDepth = lsPos.z;
    if(currentDepth > closestDepth)
    {
        FragColor = vec4(FragColor.xyz * 0.3, 1.0);
    }
}