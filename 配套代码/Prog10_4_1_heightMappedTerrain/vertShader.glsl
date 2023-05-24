#version 430

layout (location=0) in vec3 vertPos;
layout (location=1) in vec2 texCoord;
layout (location=2) in vec3 vertNormal;

out vec2 tc;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
layout (binding=0) uniform sampler2D t;	// for texture
layout (binding=1) uniform sampler2D h;	// for height map

void main(void)
{	vec4 p = vec4(vertPos,1.0) + vec4((vertNormal*((texture(h, texCoord).r)/5.0f)),1.0f);
	tc = texCoord;
	gl_Position = proj_matrix * mv_matrix * p;
}
