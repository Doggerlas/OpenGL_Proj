#version 430

in vec2 tc[];
out vec2 tcs_out[];

uniform mat4 mvp_matrix;
layout (binding=0) uniform sampler2D tex_color;
layout (vertices = 16) out;

void main(void)
{	int TL = 32;  // tessellation levels
	if (gl_InvocationID ==0)
	{	gl_TessLevelOuter[0] = TL;
		gl_TessLevelOuter[2] = TL;
		gl_TessLevelOuter[1] = TL;
		gl_TessLevelOuter[3] = TL;
		gl_TessLevelInner[0] = TL;
		gl_TessLevelInner[1] = TL;
	}
	tcs_out[gl_InvocationID] = tc[gl_InvocationID];
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}