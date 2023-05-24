#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>

using namespace std;

void init(GLFWwindow* window) { }

void display(GLFWwindow* window, double currentTime) {
	/*OpenGL����glClearColor*/
	glClearColor(1.0, 0.0, 0.0, 1.0);//ָ���������ʱ�õ���ɫֵ,(1,0,0,1)����RGBֵ�еĺ�ɫ��ĩβ��1��ʾ��͸���ȣ�
	glClear(GL_COLOR_BUFFER_BIT);//ʹ��OpenGL����glClear(GL_COLOR_BUFFER_BIT)��ʵ��ʹ�ú�ɫ����ɫ�������������
}

int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }//GLFW���ʼ��

	/*WindowHintsָ���˻��������� OpenGL�汾4.3���ݣ������汾�š�=4�����ΰ汾�š�=3��*/
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	/*����GLFW����,ָ���˴��ڵĿ��ߣ�������Ϊ��λ)�Լ����ڶ����ı���
	������û���õ�����������������ΪNULL���ֱ���������ȫ����ʾ�Լ���Դ����*/
	GLFWwindow* window = glfwCreateWindow(600, 600, "Chapter 2 - program 1", NULL, NULL);
	glfwMakeContextCurrent(window);//����GLFW���ڲ������Զ������뵱ǰ OpenGL�����Ĺ��������������������Ҫ���� glfwMakeContextCurrent()

	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }//GLEW���ʼ��
	glfwSwapInterval(1);//glfwSwapInterval()��glfwSwapBuffers()����������ֱͬ����Vsync��,GLFW����Ĭ����˫�����

	init(window);
	//��Ⱦѭ����������������display() ����
	while (!glfwWindowShouldClose(window)) {
		display(window, glfwGetTime());//��glfwGetTime()�����᷵�� GLFW��ʼ��֮�󾭹���ʱ��.����ǰʱ�䴫����display()���ã��������㱣֤�����ڲ�ͬ�����������ͬ�ٶȲ���
		glfwSwapBuffers(window);//������Ļ
		glfwPollEvents();//����������¼����簴���¼�����GLFW̽�� ��Ӧ�ùرմ��ڵ��¼������û����������Ͻǵġ���ʱ��ѭ���ͻ���ֹ
	}

	glfwDestroyWindow(window);//GLFW���ٴ���
	glfwTerminate();//��ֹ����
	exit(EXIT_SUCCESS);
}