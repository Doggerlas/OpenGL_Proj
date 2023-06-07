#version 430

in vec4 varyingColor;
out vec4 fragColor;

//  与顶点着色器相同的统一变量
//  但并不直接在当前片段着色器使用

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

//  interpolate lighted color
// (interpolation of gl_Position is automatic)

void main(void)
{	fragColor = varyingColor;
}
