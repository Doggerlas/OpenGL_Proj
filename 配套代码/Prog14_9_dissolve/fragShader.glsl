#version 430

in vec2 tc;
in vec3 originalPosition;
out vec4 fragColor;

layout (binding=0) uniform sampler3D s;
layout (binding=1) uniform sampler2D e;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform float t;

void main(void)
{
	float noise = texture(s,originalPosition/2.0+.5).x;
	
	if (noise > t)
	{ fragColor = texture(e,tc);
	}
	else
	{ discard;
	}
}
