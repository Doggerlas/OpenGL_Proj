#version 430

layout (location=0) in vec3 vertPos;
layout (location=1) in vec2 texCoord;
layout (location=2) in vec3 vertNormal;

out vec2 tc;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
layout (binding=0) uniform sampler2D t;	// 用于纹理
layout (binding=1) uniform sampler2D h;	// 用于高度图

void main(void)
{	
	// "p"是高度图所改变的顶点位置
	// 由于高度图是灰度图，因此使用其任何颜色分量
	// 都可以（我们使用"r"）。除以5.0用来调整高度
	vec4 p = vec4(vertPos,1.0) + vec4((vertNormal*((texture(h, texCoord).r)/5.0f)),1.0f);
	tc = texCoord;
	gl_Position = proj_matrix * mv_matrix * p;
}
