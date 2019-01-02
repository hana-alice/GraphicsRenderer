uniform mat4 matrix;
in vec4 inPos;
in vec4 inColor;
out vec4 fragColor;

void main()
{
	fragColor = inColor;
	gl_Position = matrix * pos;
}