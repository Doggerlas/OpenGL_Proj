#version 430

layout (quads, equal_spacing,ccw) in;

uniform mat4 mvp_matrix;
layout (binding = 0) uniform sampler2D tex_color;
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
out vec3 varyingVertPos;
out vec3 varyingLightDir; 
/*-----------------*/

in vec2 tcs_out[];
out vec2 tes_out;

void main (void)
{	vec2 tc = vec2(tcs_out[0].x + (gl_TessCoord.x)/64.0,
					tcs_out[0].y + (1.0-gl_TessCoord.y)/64.0);

	// map the tessellated grid onto the texture rectangle:
	vec4 tessellatedPoint = vec4(gl_in[0].gl_Position.x + gl_TessCoord.x/64.0, 0.0,
								gl_in[0].gl_Position.z + gl_TessCoord.y/64.0, 1.0);
	
	// add the height from the height map to the vertex:
	tessellatedPoint.y += (texture(tex_height, tc).r) / 60.0;
	
	gl_Position = mvp_matrix * tessellatedPoint;
	tes_out = tc;
	
	/*--- light stuff----*/
	varyingVertPos = (mv_matrix * tessellatedPoint).xyz;
	varyingLightDir = light.position - varyingVertPos;
}