#include "mesh.h"

#include <algorithm>
#include <iostream>

Mesh::Mesh(){
	submeshBounds.push_back(0);
}

/**
 * Recalculates this mesh's normals based on triangle orientation and stores
 * those new normals in the mesh's color data. Certain shaders (such as for
 * extrusion or bump map coordinate transforms) assume normals are normal to
 * their triangle, but many meshes average normals or use the underlying
 * mathematics of the model to achieve smooth lighting. In order to allow both,
 * there need to be 2 distinct sets of normals. When working with materials
 * and textures, a vertex's color is rarely used, so that's where these true
 * normals are stored. A custom shader can be used to access these true normals
 * and do whatever manipulations are necessary, or even do lighting with them
 * if that's desirable. This function will guarantee that there are 3
 * unique vertices per triangle. This is not strictly necessary, but since
 * many models that average vertices re-use existing vertices that have different
 * true normals, it's necessary to increase the number of vertices.
 */
void Mesh::calculateFaceNormals(){
	makeAllVerticesUnique();
	for(unsigned int i = 0; i < getNumTriangles(); ++i){
		glm::vec3 vertex1 = getVertex(triangles[(i * 3) + 0]);
		glm::vec3 vertex2 = getVertex(triangles[(i * 3) + 1]);
		glm::vec3 vertex3 = getVertex(triangles[(i * 3) + 2]);
		glm::vec3 edge1 = vertex2 - vertex1;
		glm::vec3 edge2 = vertex3 - vertex1;
		glm::vec3 faceNormal = glm::normalize(glm::cross(edge1, edge2));

		setColor(triangles[(i * 3) + 0], faceNormal);
		setColor(triangles[(i * 3) + 1], faceNormal);
		setColor(triangles[(i * 3) + 2], faceNormal);
	}
}

void Mesh::calculateTangents(){
	calculateFaceNormals();
	for(unsigned int i = 0; i < getNumTriangles(); ++i){
		glm::vec3 vertex1 = getVertex(triangles[(i * 3) + 0]);
		glm::vec3 vertex2 = getVertex(triangles[(i * 3) + 1]);
		glm::vec3 vertex3 = getVertex(triangles[(i * 3) + 2]);

		glm::vec3 edge1 = vertex2 - vertex1;
		glm::vec3 edge2 = vertex3 - vertex1;

		glm::vec2 uv1 = getTextureCoordinate(triangles[(i * 3) + 0]);
		glm::vec2 uv2 = getTextureCoordinate(triangles[(i * 3) + 1]);
		glm::vec2 uv3 = getTextureCoordinate(triangles[(i * 3) + 2]);

		glm::vec2 deltaUV1 = uv2 - uv1;
		glm::vec2 deltaUV2 = uv3 - uv1;

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		glm::vec3 tangent;

		tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent = glm::normalize(tangent);

		setTangent(triangles[(i * 3) + 0], tangent);
		setTangent(triangles[(i * 3) + 1], tangent);
		setTangent(triangles[(i * 3) + 2], tangent);
	}
}

glm::vec3 Mesh::getVertex(unsigned int i){
	return glm::vec3(vertexData[i].x,
	                 vertexData[i].y,
					 vertexData[i].z);
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
	Attribute a;
	a.x = x;
	a.y = y;
	a.z = z;
	a.nx = 0.0f;
	a.ny = 0.0f;
	a.nz = 0.0f;
	a.r = 1.0f;
	a.g = 1.0f;
	a.b = 1.0f;
	a.u = 0.0f;
	a.v = 0.0f;
	vertexData.push_back(a);
	
	return vertexData.size() - 1;
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
	vertexData[index].x = vertex.x;
	vertexData[index].y = vertex.y;
	vertexData[index].z = vertex.z;
}

/**
 * Removes a vertex and shifts all other vertices. Does not modify triangles.
 * 
 * @param index the index of the vertex to remove.
 */
void Mesh::removeVertex(unsigned int index){
	vertexData.erase(vertexData.begin() + index, vertexData.begin() + index + 1);
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

glm::vec3 Mesh::getNormal(unsigned int i){
	return glm::vec3(vertexData[i].nx,
	                 vertexData[i].ny,
					 vertexData[i].nz);
}

/**
 * Sets the normal of a vertex.
 * 
 * @param index the index of the vertex to set the normal of.
 * @param normal the new normal vector. (Does not need to be normalized.)
 */
void Mesh::setNormal(unsigned int index, glm::vec3 normal){
	normal = glm::normalize(normal);
	vertexData[index].nx = normal.x;
	vertexData[index].ny = normal.y;
	vertexData[index].nz = normal.z;
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
 * Sets the tangent of a vertex.
 * 
 * @param index the index of the vertex to set the tangent of.
 * @param tangent the new tangent vector. (Does not need to be normalized.)
 */
void Mesh::setTangent(unsigned int index, glm::vec3 tangent){
	tangent = glm::normalize(tangent);
	vertexData[index].tx = tangent.x;
	vertexData[index].ty = tangent.y;
	vertexData[index].tz = tangent.z;
}

/**
 * Sets the tangent of the last vertex. Useful for sequential calls to addVertex and setTangent.
 * 
 * @param tangent the new tangent vector. (Does not need to be normalized.)
 */
void Mesh::setTangent(glm::vec3 tangent){
	setTangent(getNumVertices() - 1, tangent);
}

glm::vec3 Mesh::getColor(unsigned int i){
	return glm::vec3(vertexData[i].r,
	                 vertexData[i].g,
					 vertexData[i].b);
}

/**
 * Sets the color of a vertex.
 * 
 * @param index the index of the vertex to set the color of.
 * @param color the new color.
 */
void Mesh::setColor(unsigned int index, glm::vec3 color){
	vertexData[index].r = color.r;
	vertexData[index].g = color.g;
	vertexData[index].b = color.b;
}

/**
 * Sets the color of the last vertex. Useful for sequential calls to addVertex and setColor.
 * 
 * @param color the new color.
 */
void Mesh::setColor(glm::vec3 color){
	setColor(getNumVertices() - 1, color);
}

glm::vec2 Mesh::getTextureCoordinate(unsigned int i){
	return glm::vec2(vertexData[i].u,
	                 vertexData[i].v);
}

/**
 * Sets the texture coordinate of a vertex.
 * 
 * @param index the index of the vertex to set the texture coordinate of.
 * @param u the x position of the new texture coordinate
 * @param v the y position of the new texture coordinate
 */
void Mesh::setTextureCoordinate(unsigned int index, float u, float v){
	vertexData[index].u = u;
	vertexData[index].v = v;
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
	return &(vertexData[0].x);
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
	return vertexData.size();
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
 * Ensures that every triangle has 3 vertices that are in no other triangles.
 * This means that the number of vertices in the mesh will either increase
 * or remain the same. This does not affect submeshes as those are based on
 * the triangle indices which are modified to fit the new vertex list, but
 * remain in the same position.
 */
void Mesh::makeAllVerticesUnique(){
	std::vector<unsigned int> usedIndices;

	for(unsigned int i = 0; i < triangles.size(); ++i){
		unsigned int oldIndex = triangles[i];
		if(std::find(usedIndices.begin(), usedIndices.end(), oldIndex) == usedIndices.end()){
			usedIndices.push_back(oldIndex);
		}
		else{
			Attribute a = vertexData[oldIndex];
			vertexData.push_back(a);
			triangles[i] = vertexData.size() - 1;
		}
	}
}

/**
 * Shrinks vertex data to minimal size while still ensuring all vertices needed
 * by triangle indices are represented. This is very expensive, but should only
 * be run once or twice per object. Not intended for use with dynamic meshes.
 */
void Mesh::compress(){
	std::vector<Attribute> newVertexData;
	std::vector<unsigned int> newTriangles;
	for(unsigned int i = 0; i < getNumTriangles() * 3; ++i){
		unsigned int j;
		for(j = 0; j < newVertexData.size(); ++j){
			if(vertexData[triangles[i]] == newVertexData[j]){
				break;
			}
		}
		newTriangles.push_back(j);
		if(j == newVertexData.size()){
			newVertexData.push_back(vertexData[triangles[i]]);
		}
	}

	triangles = newTriangles;
	vertexData = newVertexData;
}
