#include "mesh.hpp"

Mesh::Mesh(){
	defaultColor = glm::vec3(1.0f, 1.0f, 1.0f);
}

int Mesh::addVertex(glm::vec3 vertex){
	vertexData.push_back(vertex.x); // X
	vertexData.push_back(vertex.y); // Y
	vertexData.push_back(vertex.z); // Z

	vertexData.push_back(defaultColor.x); // R
	vertexData.push_back(defaultColor.y); // G
	vertexData.push_back(defaultColor.z); // B

	vertexData.push_back(0.0f); // U
	vertexData.push_back(0.0f); // V
	
	return (vertexData.size() / 8) - 1;
}

int Mesh::addVertex(glm::vec2 vertex){
	return this->addVertex(glm::vec3(vertex.x, vertex.y, 0.0f));
}

void Mesh::setVertex(unsigned int index, glm::vec3 vertex){
	unsigned int attributeIndex = vertexIndexToAttributeIndex(index);
	vertexData[attributeIndex + 0] = vertex.x;
	vertexData[attributeIndex + 1] = vertex.y;
	vertexData[attributeIndex + 2] = vertex.z;
}

glm::vec3 Mesh::removeVertex(unsigned int index){
	unsigned int attributeIndex = vertexIndexToAttributeIndex(index);
	vertexData.erase(vertexData.begin() + attributeIndex, vertexData.begin() + attributeIndex + 8);
}

int Mesh::addTriangle(unsigned int index1, unsigned int index2, unsigned int index3){
	triangles.push_back(index1);
	triangles.push_back(index2);
	triangles.push_back(index3);

	return triangles.size() / 3;
}

void Mesh::removeTriangle(unsigned int index){
	triangles.erase(triangles.begin() + (index * 3), triangles.begin() + ((index + 1) * 3));
}

void Mesh::setDefaultColor(glm::vec3 color){
	defaultColor = color;
}

void Mesh::setColor(unsigned int index, glm::vec3 color){
	unsigned int attributeIndex = vertexIndexToAttributeIndex(index);
	vertexData[attributeIndex + 3] = color.x;
	vertexData[attributeIndex + 4] = color.y;
	vertexData[attributeIndex + 5] = color.z;
}

void Mesh::setTextureCoordinate(unsigned int index, glm::vec2 coordinate){
	unsigned int attributeIndex = vertexIndexToAttributeIndex(index);
	vertexData[attributeIndex + 6] = coordinate.x;
	vertexData[attributeIndex + 7] = coordinate.y;
}

float* Mesh::getVertexData(){
	return &vertexData[0];
}

unsigned int* Mesh::getIndexData(){
	return &triangles[0];
}

unsigned int Mesh::getNumVertices(){
	return vertexData.size() / 8;
}

unsigned int Mesh::getNumTriangles(){
	return triangles.size() / 3;
}

unsigned int Mesh::vertexIndexToAttributeIndex(unsigned int index){
	return index * 8;
}