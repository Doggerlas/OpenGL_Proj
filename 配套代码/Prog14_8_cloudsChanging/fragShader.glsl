#version 430

in vec2 tc;
in vec3 originalPosition;
out vec4 fragColor;

uniform mat4 mv_matrix;	 
uniform mat4 proj_matrix;
uniform float d;

layout (binding=0) uniform sampler3D s;

void main(void)
{	
	fragColor = texture(s,vec3(tc.x, tc.y, d));  // changing d replaces fixed 0.5
}
