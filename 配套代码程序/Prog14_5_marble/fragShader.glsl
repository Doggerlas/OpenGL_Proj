#version 430

in vec3 varyingNormal;
in vec3 originalPosition;
in vec3 varyingLightDir;
in vec3 varyingVertPos;

out vec4 fragColor;

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

layout (binding=0) uniform sampler3D s;

void main(void)
{	// normalize the light, normal, and view vectors:
	vec3 L = normalize(varyingLightDir);
	vec3 N = normalize(varyingNormal);
	vec3 V = normalize(-varyingVertPos);
	
	// compute light reflection vector, with respect N:
	vec3 R = normalize(reflect(-L, N));
	
	// get the angle between the light and surface normal:
	float cosTheta = dot(L,N);
	
	// angle between the view vector and reflected light:
	float cosPhi = dot(V,R);
	
	vec4 t = texture(s,originalPosition/3.0 + 0.5);

	// compute ADS contributions (per pixel):
	fragColor = 0.7 * t * (globalAmbient + light.ambient + light.diffuse * max(cosTheta,0.0))
			+ 0.5 * light.specular * pow(max(cosPhi,0.0), material.shininess);
}
