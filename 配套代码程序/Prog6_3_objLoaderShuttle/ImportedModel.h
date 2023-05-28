#include <vector>
//通过将导入的顶点放入vec2和vec3对象的向量中，简化了加载和访问OBJ文件顶点的过程
class ImportedModel
{
private:
	// 从OBJ文件读取的数值
	int numVertices;
	// 保存为顶点属性以供后续使用的数值
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normalVecs;
public:
	ImportedModel();
	ImportedModel(const char *filePath);
	int getNumVertices();
	std::vector<glm::vec3> getVertices();
	std::vector<glm::vec2> getTextureCoords();
	std::vector<glm::vec3> getNormals();
};

class ModelImporter
{
private:
	std::vector<float> vertVals;
	std::vector<float> triangleVerts;
	std::vector<float> textureCoords;
	std::vector<float> stVals;
	std::vector<float> normals;
	std::vector<float> normVals;
public:
	ModelImporter();
	void parseOBJ(const char *filePath);//逐行读取OBJ文 件，分别处理v、vt、vn和f这4种情况
	int getNumVertices();
	std::vector<float> getVertices();
	std::vector<float> getTextureCoordinates();
	std::vector<float> getNormals();
};