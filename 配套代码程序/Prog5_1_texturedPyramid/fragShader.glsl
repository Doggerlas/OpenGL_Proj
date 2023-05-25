#version 430

in vec2 tc;
out vec4 color;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
layout (binding=0) uniform sampler2D samp;//采样器

void main(void)
{	color = texture(samp, tc);//实际执行纹理处理，我们需要修改片段着色器输出颜色的方式
}