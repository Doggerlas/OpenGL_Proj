#version 430

layout (quads, equal_spacing, ccw) in;

uniform mat4 mvp_matrix;

void main (void)
{
	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;
	gl_Position = mvp_matrix * vec4(u,0,v,1);
}