#version 430

out vec2 tc;

uniform mat4 mvp_matrix;
layout (binding = 0) uniform sampler2D tex_color;
layout (binding = 1) uniform sampler2D tex_height;
layout (binding = 2) uniform sampler2D tex_normal;

/* --- light stuff ----*/
struct PositionalLight
{ vec4 ambient; vec4 diffuse; vec4 specular; vec3 position; };
struct Material
{ vec4 ambient; vec4 diffuse; vec4 specular; float shininess; };
uniform vec4 globalAmbient;
uniform PositionalLight light;
uniform Material material;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;
/*-----------------*/

void main(void)
{	vec2 patchTexCoords[] =
		vec2[] (vec2(0,0), vec2(1,0), vec2(0,1), vec2(1,1));
	
	// compute an offset for coordinates based on which instance this is
	int x = gl_InstanceID % 64;
	int y = gl_InstanceID / 64;
	
	// texture coordinates are distributed across 64 patches	
	tc = vec2( (x+patchTexCoords[gl_VertexID].x)/64.0,
			   (63-y+patchTexCoords[gl_VertexID].y)/64.0 );
	
	// vertex locations range from -0.5 to +0.5
	gl_Position = vec4(tc.x-0.5, 0.0, (1.0-tc.y)-0.5, 1.0);
}