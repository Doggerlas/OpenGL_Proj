#version 430

in vec3 varyingLightDir;
in vec3 varyingVertPos;
in vec3 varyingNormal;
in vec3 varyingTangent;
in vec3 originalVertex;
in vec2 tc;

out vec4 fragColor;

layout (binding=0) uniform sampler2D s;
layout (binding=1) uniform sampler2D t;

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

vec3 calcNewNormal()
{
	vec3 normal = normalize(varyingNormal);
	vec3 tangent = normalize(varyingTangent);
	tangent = normalize(tangent - dot(tangent, normal) * normal);
	vec3 bitangent = cross(tangent, normal);
	mat3 tbn = mat3(tangent, bitangent, normal);
	vec3 retrievedNormal = texture(s,tc).xyz;
	retrievedNormal = retrievedNormal * 2.0 - 1.0;
	vec3 newNormal = tbn * retrievedNormal;
	newNormal = normalize(newNormal);
	return newNormal;
}

void main(void)
{	// normalize the light, normal, and view vectors:
	vec3 L = normalize(varyingLightDir);
	vec3 V = normalize(-varyingVertPos);

	vec3 N = calcNewNormal();

	// get the angle between the light and surface normal:
	float cosTheta = dot(L,N);
	
	// compute light reflection vector, with respect N:
	vec3 R = normalize(reflect(-L, N));
	
	// angle between the view vector and reflected light:
	float cosPhi = dot(V,R);

	vec4 texC = texture(t,tc);
	
	// compute ADS contributions with surface texture image:
	fragColor = globalAmbient + light.ambient * texC
	+ light.diffuse * texC * max(cosTheta,0.0)
	+ light.specular * texC * pow(max(cosPhi,0.0), material.shininess);
}
