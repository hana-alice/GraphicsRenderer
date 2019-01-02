#version 330
in vec3 inPos;
in vec4 inColor;
out vec4 fragColor;

void main()
{
	fragColor = inColor;
	gl_Position =  vec4(inPos,1.0);
}