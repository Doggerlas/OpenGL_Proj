#version 430

//声明的相应的顶点属性变量 
layout (location=0) in vec3 position;
//in	代表顶点着色器从缓冲区(VBO)读取数值
//vec3	指顶点着色器每次从缓冲区拿取3个数值，这三个数值命名为position 
//layout (location=0)	把顶点属性和特定缓冲区关联起来 说明这个顶点属性的识别号是0

uniform mat4 mv_matrix;//模型-视图矩阵
uniform mat4 proj_matrix;//投影矩阵

void main(void)
{
	gl_Position = proj_matrix * mv_matrix * vec4(position,1.0);//内置输出变量，将矩阵变换应用于传入的顶点后得到的结果
} 
