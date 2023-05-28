#include <fstream>
#include <sstream>
#include <glm\glm.hpp>
#include "ImportedModel.h"
using namespace std;

ImportedModel::ImportedModel() {}

ImportedModel::ImportedModel(const char *filePath) {
	ModelImporter modelImporter = ModelImporter();
	modelImporter.parseOBJ(filePath);//// 使用modelImporter获取顶点信息
	numVertices = modelImporter.getNumVertices();
	std::vector<float> verts = modelImporter.getVertices();
	std::vector<float> tcs = modelImporter.getTextureCoordinates();
	std::vector<float> normals = modelImporter.getNormals();

	for (int i = 0; i < numVertices; i++) {
		vertices.push_back(glm::vec3(verts[i*3], verts[i*3+1], verts[i*3+2]));
		texCoords.push_back(glm::vec2(tcs[i*2], tcs[i*2+1]));
		normalVecs.push_back(glm::vec3(normals[i*3], normals[i*3+1], normals[i*3+2]));
	}
}

int ImportedModel::getNumVertices() { return numVertices; }
std::vector<glm::vec3> ImportedModel::getVertices() { return vertices; }
std::vector<glm::vec2> ImportedModel::getTextureCoords() { return texCoords; }
std::vector<glm::vec3> ImportedModel::getNormals() { return normalVecs; }

// ---------------------------------------------------------------

ModelImporter::ModelImporter() {}

//逐行读取OBJ文 件，分别处理v、vt、vn和f这4种情况
void ModelImporter::parseOBJ(const char *filePath) {
	float x, y, z;
	string content;
	ifstream fileStream(filePath, ios::in);
	string line = "";
	while (!fileStream.eof()) {
		getline(fileStream, line);
		if (line.compare(0, 2, "v ") == 0) {// 顶点位置（"v"的情况）
			stringstream ss(line.erase(0, 1)); //使用erase()跳过初始的v、vt、vn或f字符
			ss >> x; ss >> y; ss >> z;// 提取顶点位置数值
			vertVals.push_back(x);
			vertVals.push_back(y);
			vertVals.push_back(z);
		}
		if (line.compare(0, 2, "vt") == 0) {// 纹理坐标
			stringstream ss(line.erase(0, 2));// 提取纹理坐标数值
			ss >> x; ss >> y;
			stVals.push_back(x);
			stVals.push_back(y);
		}
		if (line.compare(0, 2, "vn") == 0) {// 顶点法向量
			stringstream ss(line.erase(0, 2));// 提取法向量数值
			ss >> x; ss >> y; ss >> z;
			normVals.push_back(x);
			normVals.push_back(y);
			normVals.push_back(z);
		}
		if (line.compare(0, 2, "f ") == 0) {// 三角形面（"f"的情况）
			string oneCorner, v, t, n;
			stringstream ss(line.erase(0, 2));
			for (int i = 0; i < 3; i++) {
				getline(ss, oneCorner, ' ');// 提取三角形面引用
				stringstream oneCornerSS(oneCorner);
				getline(oneCornerSS, v, '/');
				getline(oneCornerSS, t, '/');
				getline(oneCornerSS, n, '/');

				int vertRef = (stoi(v) - 1) * 3;// "stoi"将字符串转化为整型
				int tcRef = (stoi(t) - 1) * 2;
				int normRef = (stoi(n) - 1) * 3;
				// 构建顶点向量
				triangleVerts.push_back(vertVals[vertRef]);
				triangleVerts.push_back(vertVals[vertRef + 1]);
				triangleVerts.push_back(vertVals[vertRef + 2]);
				// 构建纹理坐标向量
				textureCoords.push_back(stVals[tcRef]);
				textureCoords.push_back(stVals[tcRef + 1]);
				// 法向量的向量
				normals.push_back(normVals[normRef]);
				normals.push_back(normVals[normRef + 1]);
				normals.push_back(normVals[normRef + 2]);
			}
		}
	}
}
int ModelImporter::getNumVertices() { return (triangleVerts.size()/3); }
std::vector<float> ModelImporter::getVertices() { return triangleVerts; }
std::vector<float> ModelImporter::getTextureCoordinates() { return textureCoords; }
std::vector<float> ModelImporter::getNormals() { return normals; }