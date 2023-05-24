#version 430

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 vertNormal;
layout (location = 3) in vec3 vertTangent;

out vec3 varyingLightDir;
out vec3 varyingVertPos;
out vec3 varyingNormal;
out vec3 varyingTangent;
out vec2 tc;

layout (binding=0) uniform sampler2D t;
layout (binding=1) uniform sampler2D n;
layout (binding=2) uniform sampler2D h;

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
{	varyingNormal = (norm_matrix * vec4(vertNormal,1.0)).xyz;
	varyingTangent = (norm_matrix * vec4(vertTangent,1.0)).xyz;
	
	vec4 P1 = vec4(vertPos, 1.0);
	vec4 P2 = vec4((vertNormal*((texture(h,texCoord).r)/15.0)), 1.0);
	vec4 P = vec4((P1.xyz + P2.xyz),1.0);

	varyingVertPos = (mv_matrix * P).xyz;
	varyingLightDir = light.position - varyingVertPos;
	
	tc = texCoord;
	gl_Position = proj_matrix * mv_matrix * P;
}
