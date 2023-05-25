#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
using namespace std;

#define numVAOs 1

GLuint renderingProgram;
GLuint vao[numVAOs];

GLuint createShaderProgram() {
	//GLSL������ɫ������
	const char *vshaderSource =
		"#version 430    \n"
		"void main(void) \n"
		"{ gl_Position = vec4(0.0, 0.0, 0.0, 1.0); }";
	//GLSLƬ����ɫ������
	//const char *fshaderSource =
	//	"#version 430    \n"
	//	"out vec4 color; \n"
	//	"void main(void) \n"
	//	"{ color = vec4(0.0, 0.0, 1.0, 1.0); }";


	//��������λ�þ��������ɫ
	const char *fshaderSource =
		"#version 430 \n"
		"out vec4 color; \n"
		"void main(void) \n"
		"{if (gl_FragCoord.x < 200) color = vec4(1.0, 0.0, 0.0, 1.0); else color = vec4(0.0, 0.0, 1.0, 1.0);}";


	GLuint vShader = glCreateShader(GL_VERTEX_SHADER); //����������ɫ��
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);//����Ƭ����ɫ��
	GLuint vfprogram = glCreateProgram();//�����������

	//��GLSL������ַ������������յ���ɫ��������
	glShaderSource(vShader, 1, &vshaderSource, NULL);
	glShaderSource(fShader, 1, &fshaderSource, NULL);
	//����������ɫ��
	glCompileShader(vShader);
	glCompileShader(fShader);
	//����ɫ������������
	glAttachShader(vfprogram, vShader);
	glAttachShader(vfprogram, fShader);
	//����GLSL������ȷ�����ǵļ�����
	glLinkProgram(vfprogram);

	return vfprogram;
}

void init(GLFWwindow* window) {
	renderingProgram = createShaderProgram();
	glGenVertexArrays(numVAOs, vao);//����OpenGLҪ���VAO
	glBindVertexArray(vao[0]);
}

void display(GLFWwindow* window, double currentTime) {
	glUseProgram(renderingProgram);//�����������ѱ�����ɫ�� �ĳ�������OpenGL���߽׶Σ���GPU�ϣ�
	glPointSize(30.0f);//���ط�Χ��ע�͸þ�Ĭ��Ϊ1����
	glDrawArrays(GL_POINTS, 0, 1);//�������ߴ������,ԭʼ������GL_POINTS����������ʾһ����
}

int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(600, 600, "Chapter 2 - program 2", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	init(window);

	while (!glfwWindowShouldClose(window)) {
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}