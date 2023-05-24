#version 430

layout (location=0) in vec4 vertPos;
layout (location=1) in vec4 vertNormal;

out vec3 varyingNormal; 
out vec3 varyingLightDir;
out vec3 varyingHalfVector;

struct PositionalLight
{	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec3 position;
};
struct Material
{	vec4 ambient;  
	vec4 diffuse;  
	vec4 specular;  
	float shininess;
};

uniform vec4 globalAmbient;
uniform PositionalLight light;
uniform Material material;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;

void main(void)
{
	// output vertex positions, light, and normal vectors to the rasterizer for interpolation
	vec3 vertPos3 = (mv_matrix * vertPos).xyz;
	varyingLightDir = light.position - vertPos3;
	varyingNormal = (norm_matrix * vertNormal).xyz;
	
	// calculate the half vector (L+V)
	varyingHalfVector = normalize(varyingLightDir) + normalize(-vertPos3);
	
	gl_Position = mv_matrix * vertPos;
}
