#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D texCanvas;
void main()
{
    FragColor = vec4(texture(texCanvas, TexCoord).rgb,1.0);
    //if(gl_FragCoord.x > 0)
    //    FragColor = vec4(TexCoord, 0.0, 1.0);
}