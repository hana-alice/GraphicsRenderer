#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 2) out;

in VS_OUT{
    vec4 color;
} gs_in[];

out vec4 fColor;

void main()
{
    fColor = gs_in[0].color;
    gl_Position = gl_in[0].gl_Position + vec4(-0.1,0.0,0.0,0.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(0.1,0.0,0.0,0.0);
    EmitVertex();

    EndPrimitive();

}