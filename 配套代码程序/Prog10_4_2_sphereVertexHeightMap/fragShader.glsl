#version 430

in vec3 varyingLightDir;
in vec3 varyingVertPos;
in vec3 varyingNormal;
in vec3 varyingTangent;
in vec2 tc;

out vec4 fragColor;

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

vec3 CalcBumpedNormal()
{
	vec3 Normal = normalize(varyingNormal);
	vec3 Tangent = normalize(varyingTangent);
	Tangent = normalize(Tangent - dot(Tangent, Normal) * Normal);
	vec3 Bitangent = cross(Tangent, Normal);
	vec3 BumpMapNormal = texture(n,tc).xyz;
	BumpMapNormal = BumpMapNormal * 2.0 - 1.0;
	mat3 TBN = mat3(Tangent, Bitangent, Normal);
	vec3 NewNormal = TBN * BumpMapNormal;
	NewNormal = normalize(NewNormal);
	return NewNormal;
}

void main(void)
{	// normalize the light, normal, and view vectors:
	vec3 L = normalize(varyingLightDir);
	vec3 V = normalize(-varyingVertPos);

	//vec3 N = CalcBumpedNormal();
	vec3 N = CalcBumpedNormal();

	// get the angle between the light and surface normal:
	float cosTheta = dot(L,N);
	
	// compute light reflection vector, with respect N:
	vec3 R = normalize(reflect(-L, N));
	
	// angle between the view vector and reflected light:
	float cosPhi = dot(V,R);

	// compute ADS contributions (per pixel):
	fragColor =
		0.5 * texture(t,tc)
		+
		0.5 * (fragColor = globalAmbient * material.ambient
			+ light.ambient * material.ambient
			+ light.diffuse * material.diffuse * max(cosTheta,0.0)
			+ light.specular  * material.specular
				* pow(max(cosPhi,0.0), material.shininess));
}
