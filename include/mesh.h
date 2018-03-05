#ifndef MESH_H
#define MESH_H

#include <vector>

#include <glm/glm.hpp>

class Mesh{
	public:
		static const unsigned int ATTRIBUTE_SIZE = 14;

		Mesh();
		void calculateFaceNormals();
		void calculateTangents();

		glm::vec3 getVertex(unsigned int i);
		unsigned int addVertex(float x, float y, float z);
		unsigned int addVertex(glm::vec3 vertex);
		unsigned int addVertex(float x, float y);
		unsigned int addVertex(glm::vec2 vertex);
		unsigned int addVertices(const std::vector<glm::vec3> &vertices);
		void setVertex(unsigned int index, glm::vec3 vertex);
		void removeVertex(unsigned int index);

		int addTriangle(glm::vec3 indices);
		int addTriangle(unsigned int index1, unsigned int index2, unsigned int index3);
		int addTriangles(std::vector<glm::vec3> triangles);
		int addTriangles(std::vector<unsigned int> indices);
		void removeTriangle(unsigned int index);

		glm::vec3 getNormal(unsigned int i);
		void setNormal(unsigned int index, glm::vec3 normal);
		void setNormal(glm::vec3 normal);

		void setTangent(unsigned int index, glm::vec3 tangent);
		void setTangent(glm::vec3 tangent);

		glm::vec3 getColor(unsigned int i);
		void setColor(unsigned int index, glm::vec3 color);
		void setColor(glm::vec3 color);

		glm::vec2 getTextureCoordinate(unsigned int i);
		void setTextureCoordinate(unsigned int index, float u, float v);
		void setTextureCoordinate(unsigned int index, glm::vec2 coordinate);
		void setTextureCoordinate(float u, float v);
		void setTextureCoordinate(glm::vec2 coordinate);

		void startNewSubmeshAt(unsigned int i);
		void startNewSubmesh();
	
		float* getVertexData();
		unsigned int* getIndexData();
		unsigned int getNumVertices();
		unsigned int getNumTriangles();
		unsigned int getNumSubmeshes();
		std::vector<unsigned int> getSubmeshBounds();
		
	private:
		unsigned int vertexIndexToAttributeIndex(unsigned int index);
		void makeAllVerticesUnique();

		std::vector<float> vertexData;
		std::vector<unsigned int> triangles;
		glm::vec3 defaultColor;
		std::vector<unsigned int> submeshBounds;
};

#endif