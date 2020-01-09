#version 330 core
in vec2 TexCoords;
out float FragColor;

uniform sampler2D ssao;

void main()
{
    vec2 texelSize = 1.0/(textureSize(ssao, 0));
    float result = 0.0;
    for (int x = -2; x < 2; ++x) 
    {
        for (int y = -2; y < 2; ++y) 
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(ssao, TexCoords + offset).r;
        }
    }
    FragColor = result / (4.0 * 4.0);
}