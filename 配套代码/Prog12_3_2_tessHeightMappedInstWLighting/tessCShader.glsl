#version 430

layout (vertices = 4) out;

in vec2 tc[];
out vec2 tcs_out[];

uniform mat4 mvp_matrix;
layout (binding=0) uniform sampler2D tex_color;
layout (binding = 1) uniform sampler2D tex_height;
layout (binding = 2) uniform sampler2D tex_normal;

/*--- light stuff----*/
struct PositionalLight
{	vec4 ambient; vec4 diffuse; vec4 specular; vec3 position; };
struct Material
{	vec4 ambient; vec4 diffuse; vec4 specular; float shininess; };
uniform vec4 globalAmbient;
uniform PositionalLight light;
uniform Material material;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;
/*-----------------*/

void main(void)
{	int TL = 32;
	if (gl_InvocationID == 0)
	{ gl_TessLevelOuter[0] = TL;
	  gl_TessLevelOuter[2] = TL;
	  gl_TessLevelOuter[1] = TL;
	  gl_TessLevelOuter[3] = TL;
	  gl_TessLevelInner[0] = TL;
	  gl_TessLevelInner[1] = TL;
	}
	
	tcs_out[gl_InvocationID] = tc[gl_InvocationID];
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}