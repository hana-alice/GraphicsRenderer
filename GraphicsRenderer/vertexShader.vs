#version 330
in vec3 inPos;
in vec4 inColor;
in vec2 inTexCoord;

out vec4 fragColor;
out vec2 texCoord;

//uniform mat4 transform;

void main()
{
	fragColor = inColor;
	gl_Position = vec4(inPos,1.0);
	texCoord = inTexCoord;
}