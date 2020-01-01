#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
//in vec4 lsPos;

uniform sampler2D plane;
//uniform sampler2D shadowSmp;
void main()
{
    FragColor = texture(plane, TexCoord);
#if 0
    vec3 projCoords = lsPos.xyz/lsPos.w;
    projCoords = projCoords*0.5 + 0.5;
    float currentDepth = projCoords.z;
    //float closestDepth = texture(shadowSmp,projCoords.xy).r;
    vec2 texSize = 1/textureSize(shadowSmp,0);
    float shaodwFactor = 0.0;

    for(int x = -1; x <=1; ++x)
    {
        for(int y = -1; y <= 1; y++)
        {
            //TODO: should figure out y texSize not works here
            vec2 offset = vec2(projCoords.x + x*(1.0/1280.0), projCoords.y + y*(1.0/720.0));
            float pcfDepth = texture(shadowSmp,offset).r;
            shaodwFactor += (currentDepth > pcfDepth ? 1.0 : 0.0);
        }
    }
    shaodwFactor /= 9.0; 
    FragColor = vec4(FragColor.rgb * (1-0.7*shaodwFactor), 1.0);
    //this is Y texsize not work
    /*if(texSize.x < 1/1280.0)
        FragColor = vec4(1.0,1.0,0.0, 1.0);*/
#endif
}