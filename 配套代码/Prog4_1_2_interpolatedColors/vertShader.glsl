#version 430

layout (location=0) in vec3 position;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
//根据每个顶点的位置为每个顶点指定一种颜色，并将该颜色放在输出的顶点属性varyingColor
out vec4 varyingColor;//varyingColor不是内嵌的顶点属性 对顶点着色器来说是输出值(输出到片段着色器) 所以需要自己声明为out类型

void main(void)
{	gl_Position = proj_matrix * mv_matrix * vec4(position,1.0);
	varyingColor = vec4(position,1.0)*0.5 + vec4(0.5, 0.5, 0.5, 0.5);//将取值范围从[−1…+1]转换为[0… 1]
} 
