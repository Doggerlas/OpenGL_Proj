#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coord;
out vec2 tc;
out vec3 originalPosition;

layout (binding=0) uniform sampler3D s;
layout (binding=1) uniform sampler2D e;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform float t;

void main(void)
{	originalPosition = position;
	gl_Position = proj_matrix * mv_matrix * vec4(position,1.0);
	tc = tex_coord;
}