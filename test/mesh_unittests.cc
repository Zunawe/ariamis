#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "mesh.h"
#include "gtest/gtest.h"

TEST(MeshTests, AddVertexCount){
	Mesh m;
	m.addVertex(1, 2, 3);
	ASSERT_EQ(m.getNumVertices(), 1);
	m.addVertex(4, 5, 6);
	ASSERT_EQ(m.getNumVertices(), 2);
}

TEST(MeshTests, RemoveVertexCount){
	Mesh m;
	m.addVertex(1, 2, 3);
	ASSERT_EQ(m.getNumVertices(), 1);
	m.addVertex(4, 5, 6);
	ASSERT_EQ(m.getNumVertices(), 2);
	m.removeVertex(0);
	ASSERT_EQ(m.getNumVertices(), 1);
}

TEST(MeshTests, GetVertex){
	Mesh m;
	m.addVertex(1, 2, 3);
	glm::vec3 v = m.getVertex(0);
	EXPECT_EQ(v.x, 1.0f);
	EXPECT_EQ(v.y, 2.0f);
	EXPECT_EQ(v.z, 3.0f);
}

TEST(MeshTests, RemoveVertexCorrectness){
	Mesh m;
	m.addVertex(1, 2, 3);
	m.addVertex(4, 5, 6);
	m.removeVertex(0);
	glm::vec3 v = m.getVertex(0);
	EXPECT_EQ(v.x, 4.0f);
	EXPECT_EQ(v.y, 5.0f);
	EXPECT_EQ(v.z, 6.0f);
}

TEST(MeshTests, AddVertexOverloads){
	Mesh m;
	m.addVertex(1, 2, 3);
	m.addVertex(glm::vec3(4, 5, 6));
	m.addVertex(7, 8);
	m.addVertex(glm::vec2(9, 10));

	EXPECT_EQ(glm::vec3(1, 2, 3), m.getVertex(0));
	EXPECT_EQ(glm::vec3(4, 5, 6), m.getVertex(1));
	EXPECT_EQ(glm::vec3(7, 8, 0), m.getVertex(2));
	EXPECT_EQ(glm::vec3(9, 10, 0), m.getVertex(3));
}

TEST(MeshTests, AddVertices){
	Mesh m;

	m.addVertex(99, 99, 99);

	std::vector<glm::vec3> vertices;
	for(int i = 0; i < 5; ++i){
		vertices.push_back(glm::vec3(i));
	}
	int count = m.addVertices(vertices);

	ASSERT_EQ(count, 5);
	ASSERT_EQ(m.getNumVertices(), 6);

	EXPECT_EQ(glm::vec3(99, 99, 99), m.getVertex(0));
	EXPECT_EQ(glm::vec3(0, 0, 0), m.getVertex(1));
	EXPECT_EQ(glm::vec3(4, 4, 4), m.getVertex(5));
}

TEST(MeshTests, ManipulateNormals){
	Mesh m;

	m.addVertex(1, 2, 3);
	m.setNormal(0, glm::vec3(1, 0, 0));
	ASSERT_EQ(m.getNormal(0), glm::vec3(1, 0, 0));

	m.addVertex(4, 5, 6);
	m.setNormal(glm::vec3(0, 0, 1));
	ASSERT_EQ(m.getNormal(1), glm::vec3(0, 0, 1));
}

TEST(MeshTests, NormalizeNormals){
	Mesh m;

	m.addVertex(1, 2, 3);
	m.setNormal(glm::vec3(1, 1, 3));
	ASSERT_EQ(m.getNormal(0), glm::normalize(glm::vec3(1, 1, 3)));
}

TEST(MeshTests, UniquifyVertices){
	Mesh m;
	
	m.addVertex(0, 0, 0);
	m.addVertex(1, 1, 1);
	m.addVertex(2, 2, 2);
	m.addVertex(3, 3, 3);

	m.addTriangle(0, 1, 2);
	m.addTriangle(0, 2, 3);

	m.makeAllVerticesUnique();

	ASSERT_EQ(m.getNumVertices(), 6);
}

TEST(MeshTests, CompressVerticesRemovesExtra){
	Mesh m;
	
	m.addVertex(0, 0, 0);
	m.addVertex(1, 1, 1);
	m.addVertex(2, 2, 2);
	m.addVertex(3, 3, 3);

	m.addTriangle(0, 1, 2);

	m.compress();

	ASSERT_EQ(m.getNumVertices(), 3);
}

TEST(MeshTests, CompressVerticesRemovesRedundant){
	Mesh m;
	
	m.addVertex(0, 0, 0);
	m.addVertex(1, 1, 1);
	m.addVertex(2, 2, 2);
	m.addVertex(0, 0, 0);
	m.addVertex(2, 2, 2);
	m.addVertex(3, 3, 3);

	m.addTriangle(0, 1, 2);
	m.addTriangle(3, 4, 5);

	m.compress();

	ASSERT_EQ(m.getNumVertices(), 4);
}
