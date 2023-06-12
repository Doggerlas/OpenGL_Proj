#version 430

in vec3 varyingNormal;
in vec3 varyingLightDir;
in vec3 varyingVertPos;
// 与Phong着色相同，但添加此输入顶点属性
in vec3 originalVertex;

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
{	// normalize the light, normal, and view vectors:
	vec3 L = normalize(varyingLightDir);
	vec3 N = normalize(varyingNormal);
	vec3 V = normalize(-varyingVertPos);
	
	//  添加如下代码以扰乱传入的法向量 
	float a = 0.25;	// a 控制凸起的高度 
	float b = 100.0; // b 控制凸起的宽度 
	float x = originalVertex.x;
	float y = originalVertex.y; 
	float z = originalVertex.z;
	N.x = varyingNormal.x + a*sin(b*x);	// 使用正弦函数扰乱传入法向量	
	N.y = varyingNormal.y + a*sin(b*y);
	N.z = varyingNormal.z + a*sin(b*z); 
	N = normalize(N);
	
	// 光照计算以及输出的fragColor（未更改）现在使用扰动过的法向量N
	vec3 R = normalize(reflect(-L, N));
	
	// get the angle between the light and surface normal:
	float cosTheta = dot(L,N);
	
	// angle between the view vector and reflected light:
	float cosPhi = dot(V,R);

	// compute ADS contributions (per pixel):
	fragColor = globalAmbient * material.ambient
	+ light.ambient * material.ambient
	+ light.diffuse * material.diffuse * max(cosTheta,0.0)
	+ light.specular  * material.specular *
		pow(max(cosPhi,0.0), material.shininess);
}
