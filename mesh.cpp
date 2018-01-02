#include "mesh.hpp"

Mesh::Mesh(){
	submeshBounds.push_back(0);
}

/**
 * Creates a new vertex attribute and sets the vertex position.
 * 
 * @param x the x position of the vertex to be added.
 * @param y the y position of the vertex to be added.
 * @param z the z position of the vertex to be added.
 * @return the index of the created vertex.
 */
unsigned int Mesh::addVertex(float x, float y, float z){
	vertexData.push_back(x); // X
	vertexData.push_back(y); // Y
	vertexData.push_back(z); // Z

	vertexData.push_back(0.0f); // X (Normal)
	vertexData.push_back(0.0f); // Y (Normal)
	vertexData.push_back(0.0f); // Z (Normal)

	vertexData.push_back(1.0f); // R
	vertexData.push_back(1.0f); // G
	vertexData.push_back(1.0f); // B

	vertexData.push_back(0.0f); // U
	vertexData.push_back(0.0f); // V
	
	return (vertexData.size() / ATTRIBUTE_SIZE) - 1;
}

/**
 * Creates a new vertex attribute and sets the vertex position.
 * 
 * @param vertex the vertex position to be added.
 * @return the index of the created vertex.
 */
unsigned int Mesh::addVertex(glm::vec3 vertex){
	return addVertex(vertex.x, vertex.y, vertex.z);
}

/**
 * Creates a new vertex attribute and sets the vertex position.
 * 
 * @param x the x position of the vertex to be added.
 * @param y the y position of the vertex to be added.
 * @return the index of the created vertex.
 */
unsigned int Mesh::addVertex(float x, float y){
	return addVertex(x, y, 0.0f);
}

/**
 * Creates a new vertex attribute and sets the vertex position.
 * 
 * @param vertex the vertex position to be added.
 * @return the index of the created vertex.
 */
unsigned int Mesh::addVertex(glm::vec2 vertex){
	return addVertex(glm::vec3(vertex.x, vertex.y, 0.0f));
}

/**
 * Creates a number of new vertices.
 * 
 * @param vertices a list of vertex positions to be added.
 * @return the number of vertices added.
 */
unsigned int Mesh::addVertices(const std::vector<glm::vec3> &vertices){
	for(glm::vec3 vertex : vertices){
		addVertex(vertex);
	}

	return vertices.size();
}

/**
 * Sets the position of an existing vertex.
 * 
 * @param index the index of the vertex to modify.
 * @param vertex the new value for the vertex's position.
 */
void Mesh::setVertex(unsigned int index, glm::vec3 vertex){
	unsigned int attributeIndex = vertexIndexToAttributeIndex(index);
	vertexData[attributeIndex + 0] = vertex.x;
	vertexData[attributeIndex + 1] = vertex.y;
	vertexData[attributeIndex + 2] = vertex.z;
}

/**
 * Removes a vertex and shifts all other vertices. Does not modify triangles.
 * 
 * @param index the index of the vertex to remove.
 */
void Mesh::removeVertex(unsigned int index){
	unsigned int attributeIndex = vertexIndexToAttributeIndex(index);
	vertexData.erase(vertexData.begin() + attributeIndex, vertexData.begin() + attributeIndex + ATTRIBUTE_SIZE);
}

/**
 * Adds a new triangle to the mesh.
 * 
 * @param triangle the three indices to add.
 * @return the number of triangles in the mesh after adding.
 */
int Mesh::addTriangle(glm::vec3 indices){
	triangles.push_back(indices.x);
	triangles.push_back(indices.y);
	triangles.push_back(indices.z);

	return triangles.size() / 3;
}

/**
 * Adds a new triangle to the mesh.
 * 
 * @param index1 the first vertex in the triangle
 * @param index2 the second vertex in the triangle
 * @param index3 the third vertex in the triangle
 * @return the number of triangles in the mesh after adding.
 */
int Mesh::addTriangle(unsigned int index1, unsigned int index2, unsigned int index3){
	return addTriangle(glm::vec3(index1, index2, index3));
}

/**
 * Adds triangles to the mesh from a list of vec3s.
 * 
 * @param triangles the list of triangles to add.
 * @return the number of triangles added to the mesh.
 */
int Mesh::addTriangles(std::vector<glm::vec3> triangles){
	for(auto triangle : triangles){
		addTriangle(triangle);
	}
	return triangles.size();
}

/**
 * Adds triangles to the mesh from a list of integers.
 * 
 * @param indices the list of indices to make triangles with.
 * @return the number of triangles added to the mesh.
 */
int Mesh::addTriangles(std::vector<unsigned int> indices){
	for(unsigned int i = 0; i < indices.size() / 3; i += 3){
		addTriangle(i + 0, i + 1, i + 2);
	}
	return indices.size() / 3;
}

/**
 * Removes an entire triangle from the list of triangles.
 * 
 * @param index the index of the triangle to remove.
 */
void Mesh::removeTriangle(unsigned int index){
	triangles.erase(triangles.begin() + (index * 3), triangles.begin() + ((index + 1) * 3));
}

/**
 * Sets the normal of a vertex.
 * 
 * @param index the index of the vertex to set the normal of.
 * @param normal the new normal vector. (Does not need to be normalized.)
 */
void Mesh::setNormal(unsigned int index, glm::vec3 normal){
	unsigned int attributeIndex = vertexIndexToAttributeIndex(index);
	normal = glm::normalize(normal);
	vertexData[attributeIndex + 3] = normal.x;
	vertexData[attributeIndex + 4] = normal.y;
	vertexData[attributeIndex + 5] = normal.z;
}

/**
 * Sets the normal of the last vertex. Useful for sequential calls to addVertex and setNormal.
 * 
 * @param normal the new normal vector. (Does not need to be normalized.)
 */
void Mesh::setNormal(glm::vec3 normal){
	setNormal(getNumVertices() - 1, normal);
}

/**
 * Sets the color of a vertex.
 * 
 * @param index the index of the vertex to set the color of.
 * @param color the new color.
 */
void Mesh::setColor(unsigned int index, glm::vec3 color){
	unsigned int attributeIndex = vertexIndexToAttributeIndex(index);
	vertexData[attributeIndex + 6] = color.r;
	vertexData[attributeIndex + 7] = color.b;
	vertexData[attributeIndex + 8] = color.g;
}

/**
 * Sets the color of the last vertex. Useful for sequential calls to addVertex and setColor.
 * 
 * @param color the new color.
 */
void Mesh::setColor(glm::vec3 color){
	setNormal(getNumVertices() - 1, color);
}

/**
 * Sets the texture coordinate of a vertex.
 * 
 * @param index the index of the vertex to set the texture coordinate of.
 * @param u the x position of the new texture coordinate
 * @param v the y position of the new texture coordinate
 */
void Mesh::setTextureCoordinate(unsigned int index, float u, float v){
	unsigned int attributeIndex = vertexIndexToAttributeIndex(index);
	vertexData[attributeIndex +  9] = u;
	vertexData[attributeIndex + 10] = v;
}

/**
 * Sets the texture coordinate of a vertex.
 * 
 * @param index the index of the vertex to set the texture coordinate of.
 * @param coordinate the new texture coordinate.
 */
void Mesh::setTextureCoordinate(unsigned int index, glm::vec2 coordinate){
	setTextureCoordinate(index, coordinate.x, coordinate.y);
}

/**
 * Sets the texture coordinate of the last vertex. Useful for sequential calls to addVertex and setTextureCoordinate.
 * 
 * @param u the x position of the new texture coordinate
 * @param v the y position of the new texture coordinate
 */
void Mesh::setTextureCoordinate(float u, float v){
	setTextureCoordinate(getNumVertices() - 1, u, v);
}

/**
 * Sets the texture coordinate of the last vertex. Useful for sequential calls to addVertex and setTextureCoordinate.
 * 
 * @param coordinate the new texture coordinate.
 */
void Mesh::setTextureCoordinate(glm::vec2 coordinate){
	setTextureCoordinate(getNumVertices() - 1, coordinate);
}

/**
 * Designates the start of a new submesh at the specified triangle's index.
 * The new submesh runs from the provided start until the start of the next submesh
 * or the end of the mesh.
 * 
 * Each submesh is designated a different material, allowing for multiple
 * textures and materials on a single mesh. The data itself is kept contiguously
 * as if it were a single mesh, but the bounds are stored so they can be accessed
 * while drawing the mesh.
 * 
 * @param i the index of the triangle that begins the new submesh
 */
void Mesh::startNewSubmeshAt(unsigned int i){
	auto it = submeshBounds.begin();
	while(it != submeshBounds.end() && *it < i){
		++it;
	}
	if(*it == i){
		return;
	}
	submeshBounds.insert(it, i);
}

/**
 * Designates the start of a new submesh at the end of the mesh as it is.
 * New triangles are added to the new submesh.
 * 
 * Each submesh is designated a different material, allowing for multiple
 * textures and materials on a single mesh. The data itself is kept contiguously
 * as if it were a single mesh, but the bounds are stored so they can be accessed
 * while drawing the mesh.
 */
void Mesh::startNewSubmesh(){
	startNewSubmeshAt(getNumTriangles());
}

/**
 * Returns the pointer to the data to be sent to the GPU.
 * 
 * @return the pointer to the vertex data.
 */
float* Mesh::getVertexData(){
	return &vertexData[0];
}

/**
 * Returns the pointer to the indices to be sent to the GPU.
 * 
 * @return the pointer to the index data.
 */
unsigned int* Mesh::getIndexData(){
	return &triangles[0];
}

/**
 * Returns the number of unique vertices in the mesh. One vertex includes
 * data for position, normal, color, and texture coordinates.
 * 
 * @return the number of vertices in the mesh.
 */
unsigned int Mesh::getNumVertices(){
	return vertexData.size() / ATTRIBUTE_SIZE;
}

/**
 * Returns the number of triangles in the mesh. This is NOT equivalent to
 * getNumVertices() / 3. Any number of triangles may use the same unique vertex.
 * 
 * @return the number of triangles in the mesh.
 */
unsigned int Mesh::getNumTriangles(){
	return triangles.size() / 3;
}

/**
 * Returns the number of submeshes this mesh has. Each submesh is designated a
 * different material.
 * 
 * @return the number of submeshes in the mesh
 */
unsigned int Mesh::getNumSubmeshes(){
	return submeshBounds.size();
}

/**
 * Returns a vector containing the indices of the triangles that start new submeshes.
 * Each entry is the beginning of a submesh, and a submesh ends when the next one begins.
 * The last submesh ends after the last triangle in the mesh.
 * 
 * Each submesh is designated a different material, allowing for multiple
 * textures and materials on a single mesh. The data itself is kept contiguously
 * as if it were a single mesh, but the bounds are stored so they can be accessed
 * while drawing the mesh.
 * 
 * @return a vector of triangle indices that begin new submeshes.
 */
std::vector<unsigned int> Mesh::getSubmeshBounds(){
	return submeshBounds;
}

/**
 * Turns the conceptual index of the vertex into the index of the first float in a vertex attribute.
 */
unsigned int Mesh::vertexIndexToAttributeIndex(unsigned int index){
	return index * ATTRIBUTE_SIZE;
}
