#version 330 core
out vec4 FragColor;
in vec3 TexCoord;
uniform sampler2D texCanvas;
void main()
{
    FragColor = texture(texCanvas, TexCoord.xy);
}