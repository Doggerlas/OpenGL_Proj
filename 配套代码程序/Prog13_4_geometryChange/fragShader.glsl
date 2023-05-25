#version 430

in vec3 varyingVertPosG;
in vec3 varyingLightDirG;
in vec3 varyingNormalG;
 
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

void main(void)
{
	// normalized the light, normal, and eye direction vectors
	vec3 L = normalize(varyingLightDirG);
	vec3 N = normalize(varyingNormalG);
	vec3 V = normalize(-varyingVertPosG);
	
	// compute light reflection vector, with respect N:
	vec3 R = normalize(reflect(-L, N));
	
	// get the angle between the light and surface normal
	float cosTheta = dot(L,N);
	
	// angle between the view vector and reflected light:
	float cosPhi = dot(V,R);

	fragColor = globalAmbient * material.ambient  +  light.ambient * material.ambient
				+ light.diffuse * material.diffuse * max(cosTheta,0.0)
				+ light.specular * material.specular * pow(max(cosPhi,0.0), material.shininess);
}
