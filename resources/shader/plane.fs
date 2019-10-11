#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
in vec4 lsPos;

uniform sampler2D plane;
uniform sampler2D shadowSmp;
void main()
{
    FragColor = texture(plane, TexCoord);

    vec3 projCoords = lsPos.xyz/lsPos.w;
    projCoords = projCoords*0.5 + 0.5;
    float closestDepth = texture(shadowSmp,projCoords.xy).r;
    float currentDepth = projCoords.z;
    if(currentDepth > closestDepth)
    {
        FragColor = vec4(FragColor.rgb * 0.3, 1.0);
    }

}