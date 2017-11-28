#ifndef MESH_H
#define MESH_H

#include <vector>
#include <glm/glm.hpp>

class Mesh{
	public:
		Mesh();
		unsigned int addVertex(glm::vec3 vertex);
		unsigned int addVertex(glm::vec2 vertex);
		unsigned int addVertices(const std::vector<glm::vec3> &vertices);
		void setVertex(unsigned int index, glm::vec3 vertex);
		glm::vec3 removeVertex(unsigned int index);
		int addTriangle(unsigned int index1, unsigned int index2, unsigned int index3);
		void removeTriangle(unsigned int index);
		void setNormal(unsigned int index, glm::vec3 normal);
		void setNormal(glm::vec3 normal);
		void setTextureCoordinate(unsigned int index, glm::vec2 coordinate);
	
		float* getVertexData();
		unsigned int* getIndexData();
		unsigned int getNumVertices();
		unsigned int getNumTriangles();
		
	private:
		std::vector<float> vertexData;
		std::vector<unsigned int> triangles;
		glm::vec3 defaultColor;

		unsigned int vertexIndexToAttributeIndex(unsigned int index);
};

#endif