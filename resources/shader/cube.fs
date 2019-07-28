#version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec3 Position;
//in vec2 TexCoord;
uniform vec3 camPos;
uniform samplerCube texture1;
//"uniform sampler2D texture2;
void main()
{
    vec3 I = normalize(Position - camPos);
    vec3 R = reflect(I,normalize(Normal));
    FragColor = texture(texture1, R);
};