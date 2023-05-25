#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL2\soil2.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp> // glm::value_ptr
#include <glm\gtc\matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "HalfSphere.h"
#include "Utils.h"
using namespace std;

float toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

#define numVAOs 1
#define numVBOs 3

/*	Simulates drifting clouds.
	To view the skydome from the inside, move the camera to 0,2,0
	and change the winding order to CW.
*/

float cameraX, cameraY, cameraZ;
float objLocX, objLocY, objLocZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

HalfSphere halfSphere(48);
int numSphereVertices;

GLuint skyTexture;
const int noiseHeight = 200;
const int noiseWidth = 200;
const int noiseDepth = 200;
double noise[noiseHeight][noiseWidth][noiseDepth];

// variable allocation for display
GLuint mvLoc, projLoc;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat;

// 3D Noise Texture section

double smoothNoise(double x1, double y1, double z1) {
	//get fractional part of x, y, and z
	double fractX = x1 - (int)x1;
	double fractY = y1 - (int)y1;
	double fractZ = z1 - (int)z1;

	//neighbor values
	int x2 = ((int)x1 + noiseWidth + 1) % noiseWidth;
	int y2 = ((int)y1 + noiseHeight + 1) % noiseHeight;
	int z2 = ((int)z1 + noiseDepth + 1) % noiseDepth;

	//smooth the noise by interpolating
	double value = 0.0;
	value += (1-fractX) * (1-fractY) * (1-fractZ) * noise[(int)x1][(int)y1][(int)z1];
	value += (1-fractX) * fractY     * (1-fractZ) * noise[(int)x1][(int)y2][(int)z1];
	value += fractX     * (1-fractY) * (1-fractZ) * noise[(int)x2][(int)y1][(int)z1];
	value += fractX     * fractY     * (1-fractZ) * noise[(int)x2][(int)y2][(int)z1];

	value += (1-fractX) * (1-fractY) * fractZ     * noise[(int)x1][(int)y1][(int)z2];
	value += (1-fractX) * fractY     * fractZ     * noise[(int)x1][(int)y2][(int)z2];
	value += fractX     * (1-fractY) * fractZ     * noise[(int)x2][(int)y1][(int)z2];
	value += fractX     * fractY     * fractZ     * noise[(int)x2][(int)y2][(int)z2];

	return value;
}

double logistic(double x) {
	double k = 0.2; // tunable haziness of clouds
	return (1.0 / (1.0 + pow(2.718, -k*x)));
}

double turbulence(double x, double y, double z, double size) {
	double value = 0.0, initialSize = size, cloudQuant;
	while (size >= 0.9) {
		value = value + smoothNoise(x / size, y / size, z / size) * size;
		size = size / 2.0;
	}
	cloudQuant = 130.0; // tunable quantity of clouds
	value = value / initialSize;
	value = 256.0 * logistic(value * 128.0 - cloudQuant);
	//value = 128.0 * value / initialSize;
	return value;
}

void fillDataArray(GLubyte data[]) {
	for (int i = 0; i<noiseHeight; i++) {
		for (int j = 0; j<noiseWidth; j++) {
			for (int k = 0; k<noiseDepth; k++) {
				float brightness = 1.0f - (float)turbulence(i, j, k, 32) / 256.0f;

				float redPortion = brightness*255.0f;
				float greenPortion = brightness*255.0f;
				float bluePortion = 1.0f*255.0f;

				data[i*(noiseWidth*noiseHeight * 4) + j*(noiseHeight * 4) + k * 4 + 0] = (GLubyte)redPortion;
				data[i*(noiseWidth*noiseHeight * 4) + j*(noiseHeight * 4) + k * 4 + 1] = (GLubyte)greenPortion;
				data[i*(noiseWidth*noiseHeight * 4) + j*(noiseHeight * 4) + k * 4 + 2] = (GLubyte)bluePortion;
				data[i*(noiseWidth*noiseHeight * 4) + j*(noiseHeight * 4) + k * 4 + 3] = (GLubyte)0;
			}
		}
	}
}

GLuint buildNoiseTexture() {
	GLuint textureID;
	GLubyte* data = new GLubyte[noiseHeight*noiseWidth*noiseDepth * 4];

	fillDataArray(data);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_3D, textureID);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexStorage3D(GL_TEXTURE_3D, 1, GL_RGBA8, noiseWidth, noiseHeight, noiseDepth);
	glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, noiseWidth, noiseHeight, noiseDepth, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, data);

	return textureID;
}

void generateNoise() {
	for (int x = 0; x<noiseHeight; x++) {
		for (int y = 0; y<noiseWidth; y++) {
			for (int z = 0; z<noiseDepth; z++) {
				noise[x][y][z] = (double)rand() / (RAND_MAX + 1.0);
			}
		}
	}
}

// model section

void setupVertices(void) {
	std::vector<int> ind = halfSphere.getIndices();
	std::vector<glm::vec3> vert = halfSphere.getVertices();
	std::vector<glm::vec2> tex = halfSphere.getTexCoords();
	std::vector<glm::vec3> norm = halfSphere.getNormals();
	numSphereVertices = halfSphere.getNumIndices();

	std::vector<float> pvalues;
	std::vector<float> tvalues;
	std::vector<float> nvalues;

	for (int i = 0; i < halfSphere.getNumIndices(); i++) {
		pvalues.push_back((vert[ind[i]]).x);
		pvalues.push_back((vert[ind[i]]).y);
		pvalues.push_back((vert[ind[i]]).z);
		tvalues.push_back((tex[ind[i]]).x);
		tvalues.push_back((tex[ind[i]]).y);
		nvalues.push_back((norm[ind[i]]).x);
		nvalues.push_back((norm[ind[i]]).y);
		nvalues.push_back((norm[ind[i]]).z);
	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);
}

void init(GLFWwindow* window) {
	renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");
	cameraX = 0.0f; cameraY = 2.0f; cameraZ = 10.0f;
	objLocX = 0.0f; objLocY = 0.0f; objLocZ = 0.0f;

	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);

	setupVertices();

	generateNoise();
	skyTexture = buildNoiseTexture();
}

void display(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram);

	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(objLocX, objLocY, objLocZ));
	mMat = glm::scale(glm::mat4(1.0f), glm::vec3(4.0f, 4.0f, 4.0f));
	mvMat = vMat * mMat;

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, skyTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, numSphereVertices);
}

void window_size_callback(GLFWwindow* win, int newWidth, int newHeight) {
	aspect = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}

int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(800, 800, "Chapter14 - program7", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	glfwSetWindowSizeCallback(window, window_size_callback);

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