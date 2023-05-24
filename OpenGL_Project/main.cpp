#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader_Cus.h"
#include <iostream>
#include <math.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <sys/timeb.h>
#include <time.h>
#include <sys/timeb.h>
#include <Windows.h>

#define MATH_PI 3.1415926535897932384626433832802

const unsigned int screen_width = 600;
const unsigned int screen_height = 600;


typedef unsigned int uint;

//void UpdateMVPMatrix(glm::mat4& mvpMatrix, int angleX, int angleY, float ratio);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Shader ourShader("1.vert", "1.frag");

	GLint m_SamplerLoc = GL_NONE;
	GLint m_MVPMatLoc = GL_NONE;
	GLint m_TimeLoc = GL_NONE;
	GLint m_SizeLoc = GL_NONE;

	//m_SamplerLoc = glGetUniformLocation(m_ProgramObj, "s_TextureMap");
	//m_MVPMatLoc = glGetUniformLocation(ourShader.ID, "u_MVPMatrix");
	m_SizeLoc = glGetUniformLocation(ourShader.ID, "u_screenSize");
	m_TimeLoc = glGetUniformLocation(ourShader.ID, "u_time");


	GLfloat verticesCoords[] = {
			-1.0f,  1.0f, 0.0f,  // Position 0
			-1.0f, -1.0f, 0.0f,  // Position 1
			1.0f,  -1.0f, 0.0f,  // Position 2
			1.0f,   1.0f, 0.0f,  // Position 3
	};

	GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

	glm::mat4 m_MVPMatrix;
	int m_AngleX = 0;
	int m_AngleY = 0;
	float m_ScaleX = 1.0f;
	float m_ScaleY = 1.0f;

	GLuint m_VaoId, m_VboIds[3];
	// Generate VBO Ids and load the VBOs with data
	glGenBuffers(3, m_VboIds);
	glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCoords), verticesCoords, GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VboIds[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Generate VAO Id
	glGenVertexArrays(1, &m_VaoId);
	glBindVertexArray(m_VaoId);

	glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (const void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

	//	glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[1]);
	//	glEnableVertexAttribArray(1);
	//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (const void *)0);
	//	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VboIds[2]);

	glBindVertexArray(GL_NONE);

	while (!glfwWindowShouldClose(window))
	{
		//UpdateMVPMatrix(m_MVPMatrix, m_AngleX, m_AngleY, (float)screen_width / screen_height);

		glUseProgram(ourShader.ID);

		glBindVertexArray(m_VaoId);

		//glUniformMatrix4fv(m_MVPMatLoc, 1, GL_FALSE, &m_MVPMatrix[0][0]);
		float fTime = static_cast<float>(fmod(GetTickCount(), 2000) / 2000);
		float fTime1 = static_cast<float>(glfwGetTime());

		//std::cout << "fTime1: " << fTime1 << std::endl;

		glUniform1f(m_TimeLoc, fTime1);
		glUniform2f(m_SizeLoc, screen_width, screen_height);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const void*)0);
		//交换缓冲并且检查是否有触发事件(比如键盘输入、鼠标移动)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(ourShader.ID);
	glDeleteBuffers(3, m_VboIds);
	glDeleteVertexArrays(1, &m_VaoId);

	glfwTerminate();
	return 0;
}
