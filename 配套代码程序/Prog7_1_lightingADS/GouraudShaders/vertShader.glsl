#version 430

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertNormal;
out vec4 varyingColor;

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
uniform mat4 norm_matrix;// 用来变换法向量

void main(void)
{	vec4 color;
	
	// 将顶点位置转换到视觉空间
	vec4 P = mv_matrix * vec4(vertPos,1.0);
	
	//Gouraud着色算法
	// 将法向量转换到视觉空间
	vec3 N = normalize((norm_matrix * vec4(vertNormal,1.0)).xyz);//这是将vec4转换为仅包含其前3个元素的等效vec3的快捷方式。
	
	// 计算视觉空间光照向量(从顶点到光源)
	vec3 L = normalize(light.position - P.xyz);
	
	//  视觉向量等于视觉空间中的负顶点位置
	vec3 V = normalize(-P.xyz);
	
	//  R是-L的相对于表面向量N的镜像
	vec3 R = reflect(-L,N);

	// 环境光、漫反射和镜面反射分量
	vec3 ambient =
		((globalAmbient * material.ambient)
		+ (light.ambient * material.ambient)).xyz;
		
	vec3 diffuse =
		light.diffuse.xyz * material.diffuse.xyz
		* max(dot(N,L), 0.0);
		
	vec3 specular =
		pow(max(dot(R,V), 0.0f), material.shininess)
		* material.specular.xyz * light.specular.xyz;

	// 将颜色输出发送到片段着色器
	varyingColor = vec4((ambient + diffuse + specular), 1.0);
	
	// 将位置发送到片段着色器，如前
	gl_Position = proj_matrix * mv_matrix * vec4(vertPos,1.0);
}
