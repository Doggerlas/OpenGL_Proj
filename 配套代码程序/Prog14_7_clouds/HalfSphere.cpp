#include <cmath>
#include <vector>
#include <iostream>
#include <glm\glm.hpp>
#include "HalfSphere.h"
using namespace std;

HalfSphere::HalfSphere() {
	init(48);
}

HalfSphere::HalfSphere(int prec) {
	init(prec);
}

float HalfSphere::toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

void HalfSphere::init(int prec) {
	numVertices = (prec + 1) * (prec + 1);
	numIndices = prec * prec * 6;
	for (int i = 0; i < numVertices; i++) { vertices.push_back(glm::vec3()); }
	for (int i = 0; i < numVertices; i++) { texCoords.push_back(glm::vec2()); }
	for (int i = 0; i < numVertices; i++) { normals.push_back(glm::vec3()); }
	for (int i = 0; i < numIndices; i++) { indices.push_back(0); }

	// calculate triangle vertices
	for (int i = 0; i <= prec; i++) {
		for (int j = 0; j <= prec; j++) {
			float y = (float)cos(toRadians(90.0f - i * 90.0f / prec));
			float x = -(float)cos(toRadians(j*360.0f / prec))*(float)abs(cos(asin(y)));
			float z = (float)sin(toRadians(j*360.0f / (float)(prec)))*(float)abs(cos(asin(y)));
			vertices[i*(prec + 1) + j] = glm::vec3(x, y, z);
			texCoords[i*(prec + 1) + j] = glm::vec2(((float)j / prec), ((float)i / prec));
			normals[i*(prec + 1) + j] = glm::vec3(x, y, z);
		}
	}
	// calculate triangle indices
	for (int i = 0; i<prec; i++) {
		for (int j = 0; j<prec; j++) {
			indices[6 * (i*prec + j) + 0] = i*(prec + 1) + j;
			indices[6 * (i*prec + j) + 1] = i*(prec + 1) + j + 1;
			indices[6 * (i*prec + j) + 2] = (i + 1)*(prec + 1) + j;
			indices[6 * (i*prec + j) + 3] = i*(prec + 1) + j + 1;
			indices[6 * (i*prec + j) + 4] = (i + 1)*(prec + 1) + j + 1;
			indices[6 * (i*prec + j) + 5] = (i + 1)*(prec + 1) + j;
		}
	}
}

int HalfSphere::getNumVertices() { return numVertices; }
int HalfSphere::getNumIndices() { return numIndices; }
std::vector<int> HalfSphere::getIndices() { return indices; }
std::vector<glm::vec3> HalfSphere::getVertices() { return vertices; }
std::vector<glm::vec2> HalfSphere::getTexCoords() { return texCoords; }
std::vector<glm::vec3> HalfSphere::getNormals() { return normals; }