#ifndef MESH_H
#define MESH_H

#include <vector>
#include <glm/glm.hpp>

class Mesh{
	public:
		Mesh();
		int addVertex(glm::vec3 vertex);
		int addVertex(glm::vec2 vertex);
		void setVertex(unsigned int index, glm::vec3 vertex);
		glm::vec3 removeVertex(unsigned int index);
		int addTriangle(unsigned int index1, unsigned int index2, unsigned int index3);
		void removeTriangle(unsigned int index);
		void setDefaultColor(glm::vec3 color);
		void setColor(unsigned int index, glm::vec3 color);
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