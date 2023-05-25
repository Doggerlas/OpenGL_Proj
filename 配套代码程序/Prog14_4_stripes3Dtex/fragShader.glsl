#version 430

in vec3 originalPosition;
out vec4 fragColor;

uniform mat4 mv_matrix;	 
uniform mat4 proj_matrix;

layout (binding=0) uniform sampler3D s;

void main(void)
{	
	fragColor = texture(s, originalPosition/2.0+.5);
}
