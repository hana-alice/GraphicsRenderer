#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D smp;
void main()
{
    FragColor = texture(smp, TexCoord);
}