#version 430

layout (vertices = 4) out;

in vec2 tc[];
out vec2 tcs_out[];

uniform mat4 mvp_matrix;
layout (binding=0) uniform sampler2D tex_color;
layout (binding = 1) uniform sampler2D tex_height;

void main(void)
{	float subdivisions = 16.0;
	if (gl_InvocationID == 0)
	{ 	vec4 p0 = mvp_matrix * gl_in[0].gl_Position;  p0 = p0 / p0.w;
		vec4 p1 = mvp_matrix * gl_in[1].gl_Position;  p1 = p1 / p1.w;
		vec4 p2 = mvp_matrix * gl_in[2].gl_Position;  p2 = p2 / p2.w;
		float width  = length(p1-p0) * subdivisions + 1.0;
		float height = length(p2-p0) * subdivisions + 1.0;
		gl_TessLevelOuter[0] = height;
		gl_TessLevelOuter[1] = width;
		gl_TessLevelOuter[2] = height;
		gl_TessLevelOuter[3] = width;
		gl_TessLevelInner[0] = width;
		gl_TessLevelInner[1] = height;
	}
	
	tcs_out[gl_InvocationID] = tc[gl_InvocationID];
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}