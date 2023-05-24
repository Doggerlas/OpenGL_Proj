#version 430

layout (triangles) in;

in vec3 varyingNormal[];

out vec3 varyingVertPosG;
out vec3 varyingLightDirG;
out vec3 varyingNormalG;

layout (line_strip, max_vertices=2) out;

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

float sLen = 0.1;

void main(void)
{	// offset the three triangle vertices by the surface normal
	vec3 op0 = gl_in[0].gl_Position.xyz;
	vec3 op1 = gl_in[1].gl_Position.xyz;
	vec3 op2 = gl_in[2].gl_Position.xyz;
	vec3 ep0 = gl_in[0].gl_Position.xyz + varyingNormal[0]*sLen;
	vec3 ep1 = gl_in[1].gl_Position.xyz + varyingNormal[1]*sLen;
	vec3 ep2 = gl_in[2].gl_Position.xyz + varyingNormal[2]*sLen;
	
	// compute the new points comprising a small line segment
	vec3 newPoint1 = (op0 + op1 + op2)/3.0;	// start point
	vec3 newPoint2 = (ep0 + ep1 + ep2)/3.0;	// end point
	
	gl_Position = proj_matrix * vec4(newPoint1, 1.0);
	varyingVertPosG = newPoint1;
	varyingLightDirG = light.position - newPoint1;
	varyingNormalG = varyingNormal[0];
	EmitVertex();
	
	gl_Position = proj_matrix * vec4(newPoint2, 1.0);
	varyingVertPosG = newPoint2;
	varyingLightDirG = light.position - newPoint2;
	varyingNormalG = varyingNormal[1];
	EmitVertex();
	
	EndPrimitive();
}
