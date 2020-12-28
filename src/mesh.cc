#include "mesh.h"

#include <iostream>

Mesh::Mesh(){
	submeshBounds.push_back(0);
}

void Mesh::uniquifyVertices(){
    std::vector<Vertex> newVertices;
    std::vector<unsigned int> newIndices;
	for(unsigned int i = 0; i < triangles.size(); ++i){
        newVertices.push_back(vertices[triangles[i]]);
        newIndices.push_back(newVertices.size() - 1);
	}

    vertices = newVertices;
    triangles = newIndices;
}

/**
 * Shrinks vertex data to minimal size while still ensuring all vertices needed
 * by triangle indices are represented. This is very expensive, but should only
 * be run once or twice per object. Not intended for use with dynamic meshes.
 */
void Mesh::compress(){
	std::vector<Vertex> newVertices;
	std::vector<unsigned int> newTriangles;
	for(unsigned int i = 0; i < triangles.size(); ++i){
		unsigned int j;
		for(j = 0; j < newVertices.size(); ++j){
			if(vertices[triangles[i]] == newVertices[j]){
				break;
			}
		}
		newTriangles.push_back(j);
		if(j == newVertices.size()){
			newVertices.push_back(vertices[triangles[i]]);
		}
	}

	triangles = newTriangles;
	vertices = newVertices;
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
	uniquifyVertices();
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

glm::vec3 Mesh::getVertex(unsigned int i){
	return glm::vec3(vertices[i].x,
	                 vertices[i].y,
					 vertices[i].z);
}

/**
 * Creates a new vertex and sets the position.
 * 
 * @param x the x position of the vertex to be added.
 * @param y the y position of the vertex to be added.
 * @param z the z position of the vertex to be added.
 * @return the index of the created vertex.
 */
unsigned int Mesh::addVertex(float x, float y, float z){
	Vertex v;
	v.x = x;
	v.y = y;
	v.z = z;
	v.nx = 0.0f;
	v.ny = 0.0f;
	v.nz = 0.0f;
	v.r = 0.0f;
	v.g = 0.0f;
	v.b = 0.0f;
	v.u = 0.0f;
	v.v = 0.0f;
	vertices.push_back(v);
	
	return vertices.size() - 1;
}

/**
 * Creates a new vertex and sets the position.
 * 
 * @param v the vertex position to be added.
 * @return the index of the created vertex.
 */
unsigned int Mesh::addVertex(glm::vec3 v){
	return addVertex(v.x, v.y, v.z);
}

/**
 * Creates a new vertex and sets the position.
 * 
 * @param x the x position of the vertex to be added.
 * @param y the y position of the vertex to be added.
 * @return the index of the created vertex.
 */
unsigned int Mesh::addVertex(float x, float y){
	return addVertex(x, y, 0.0f);
}

/**
 * Creates a new vertex and sets the position.
 * 
 * @param v the vertex position to be added.
 * @return the index of the created vertex.
 */
unsigned int Mesh::addVertex(glm::vec2 v){
	return addVertex(glm::vec3(v.x, v.y, 0.0f));
}

/**
 * Creates a number of new vertices.
 * 
 * @param vertices a list of vertex positions to be added.
 * @return the number of vertices added.
 */
unsigned int Mesh::addVertices(const std::vector<glm::vec3> &vertices){
	for(glm::vec3 v : vertices){
		addVertex(v);
	}

	return vertices.size();
}

/**
 * Removes a vertex and shifts all other vertices. Does not modify triangles.
 * 
 * @param i the index of the vertex to remove.
 */
void Mesh::removeVertex(unsigned int i){
	vertices.erase(vertices.begin() + i, vertices.begin() + i + 1);
}

/**
 * Sets the position of an existing vertex.
 * 
 * @param i the index of the vertex to modify.
 * @param vertex the new value for the vertex's position.
 */
void Mesh::setVertexPosition(unsigned int i, glm::vec3 v){
	vertices[i].x = v.x;
	vertices[i].y = v.y;
	vertices[i].z = v.z;
}

/**
 * Sets the position of an existing vertex.
 * 
 * @param vertex the new value for the vertex's position.
 */
void Mesh::setVertexPosition(glm::vec3 v){
	setVertexPosition(getNumVertices() - 1, v);
}

glm::vec3 Mesh::getNormal(unsigned int i){
	return glm::vec3(vertices[i].nx,
	                 vertices[i].ny,
					 vertices[i].nz);
}

/**
 * Sets the normal of a vertex.
 * 
 * @param i the index of the vertex to set the normal of.
 * @param n the new normal vector. (Does not need to be normalized.)
 */
void Mesh::setNormal(unsigned int i, glm::vec3 n){
	n = glm::normalize(n);
	vertices[i].nx = n.x;
	vertices[i].ny = n.y;
	vertices[i].nz = n.z;
}

/**
 * Sets the normal of the last vertex. Useful for sequential calls to addVertex and setNormal.
 * 
 * @param n the new normal vector. (Does not need to be normalized.)
 */
void Mesh::setNormal(glm::vec3 n){
	setNormal(getNumVertices() - 1, n);
}

glm::vec3 Mesh::getColor(unsigned int i){
	return glm::vec3(vertices[i].r,
	                 vertices[i].g,
					 vertices[i].b);
}

/**
 * Sets the color of a vertex.
 * 
 * @param i the index of the vertex to set the color of.
 * @param c the new color.
 */
void Mesh::setColor(unsigned int i, glm::vec3 c){
	vertices[i].r = c.r;
	vertices[i].g = c.g;
	vertices[i].b = c.b;
}

/**
 * Sets the color of the last vertex. Useful for sequential calls to addVertex and setColor.
 * 
 * @param c the new color.
 */
void Mesh::setColor(glm::vec3 c){
	setColor(getNumVertices() - 1, c);
}

glm::vec2 Mesh::getTextureCoordinate(unsigned int i){
	return glm::vec2(vertices[i].u,
	                 vertices[i].v);
}

/**
 * Sets the texture coordinate of a vertex.
 * 
 * @param i the index of the vertex to set the texture coordinate of.
 * @param u the x position of the new texture coordinate
 * @param v the y position of the new texture coordinate
 */
void Mesh::setTextureCoordinate(unsigned int i, float u, float v){
	vertices[i].u = u;
	vertices[i].v = v;
}

/**
 * Sets the texture coordinate of a vertex.
 * 
 * @param i the index of the vertex to set the texture coordinate of.
 * @param coordinate the new texture coordinate.
 */
void Mesh::setTextureCoordinate(unsigned int i, glm::vec2 coordinate){
	setTextureCoordinate(i, coordinate.x, coordinate.y);
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
 * @param i1 the first vertex in the triangle
 * @param i2 the second vertex in the triangle
 * @param i3 the third vertex in the triangle
 * @return the number of triangles in the mesh after adding.
 */
int Mesh::addTriangle(unsigned int i1, unsigned int i2, unsigned int i3){
	return addTriangle(glm::vec3(i1, i2, i3));
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
 * @param i the index of the triangle to remove.
 */
void Mesh::removeTriangle(unsigned int i){
	triangles.erase(triangles.begin() + (i * 3), triangles.begin() + ((i + 1) * 3));
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
	return &(vertices[0].x);
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
 * Returns the number of unique vertices in the mesh. One vertex includes
 * data for position, normal, color, and texture coordinates.
 * 
 * @return the number of vertices in the mesh.
 */
unsigned int Mesh::getNumVertices(){
	return vertices.size();
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
