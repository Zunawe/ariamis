#ifndef MESH_H
#define MESH_H

#include <vector>

#include <glm/glm.hpp>

class Mesh{
	private:
		struct Vertex{
			float x;
			float y;
			float z;
			float nx;
			float ny;
			float nz;
			float r;
			float b;
			float g;
			float u;
			float v;

			bool operator==(const Vertex &v) const{
				return this->x == v.x &&
					   this->y == v.y &&
					   this->z == v.z &&
					   this->nx == v.nx &&
					   this->ny == v.ny &&
					   this->nz == v.nz &&
					   this->r == v.r &&
					   this->g == v.g &&
					   this->b == v.b &&
					   this->u == v.u &&
					   this->v == v.v;
			}

			bool operator!=(const Vertex &v) const{
				return !(*this == v);
			}

			Vertex& operator=(const Vertex &v){
				this->x = v.x;
				this->y = v.y;
				this->z = v.z;
				this->nx = v.nx;
				this->ny = v.ny;
				this->nz = v.nz;
				this->r = v.r;
				this->g = v.g;
				this->b = v.b;
				this->u = v.u;
				this->v = v.v;

				return *this;
			}
		};
	public:
		static const unsigned int VERTEX_SIZE = sizeof(Vertex);

		Mesh();
		void uniquifyVertices();
		void compress();
		void calculateFaceNormals(bool saveToColor);

		glm::vec3 getVertex(unsigned int i);
		unsigned int addVertex(float x, float y, float z);
		unsigned int addVertex(glm::vec3 v);
		unsigned int addVertex(float x, float y);
		unsigned int addVertex(glm::vec2 v);
		unsigned int addVertices(const std::vector<glm::vec3> &vertices);
		void removeVertex(unsigned int i);

		void setVertexPosition(unsigned int i, glm::vec3 v);
		void setVertexPosition(glm::vec3 v);

		glm::vec3 getNormal(unsigned int i);
		void setNormal(unsigned int i, glm::vec3 n);
		void setNormal(glm::vec3 n);

		glm::vec3 getColor(unsigned int i);
		void setColor(unsigned int i, glm::vec3 c);
		void setColor(glm::vec3 c);

		glm::vec2 getTextureCoordinate(unsigned int i);
		void setTextureCoordinate(unsigned int i, float u, float v);
		void setTextureCoordinate(unsigned int i, glm::vec2 coordinate);
		void setTextureCoordinate(float u, float v);
		void setTextureCoordinate(glm::vec2 coordinate);

		int addTriangle(glm::vec3 indices);
		int addTriangle(unsigned int i1, unsigned int i2, unsigned int i3);
		int addTriangles(std::vector<glm::vec3> triangles);
		int addTriangles(std::vector<unsigned int> indices);
		void removeTriangle(unsigned int i);

		void startNewSubmeshAt(unsigned int i);
		void startNewSubmesh();
	
		float* getVertexData();
		unsigned int* getIndexData();
		std::vector<unsigned int> getSubmeshBounds();
		unsigned int getNumVertices();
		unsigned int getNumTriangles();
		unsigned int getNumSubmeshes();
		
	private:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> triangles;
		std::vector<unsigned int> submeshBounds;
};

#endif