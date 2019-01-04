#version 330
in vec4 fragColor;
in vec2 texCoord;

uniform sampler2D ourTexture;
void main()
{
	gl_FragColor = texture(ourTexture,texCoord)* fragColor;
}