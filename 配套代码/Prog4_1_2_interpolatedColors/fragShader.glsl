#version 430

in vec4 varyingColor;// 对片段着色器来说是输入值(输入到片段着色器) 所以需要自己声明为in类型
out vec4 color;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

void main(void)
{	color = varyingColor;
}
