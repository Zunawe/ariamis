#ifndef OBJECT_RENDERER_H
#define OBJECT_RENDERER_H

#include <algorithm>

#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

#include "mesh.hpp"
#include "shader_program.hpp"
#include "texture.hpp"
#include "camera.hpp"
#include "material.hpp"

struct Light{
	glm::vec3 pos;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

class ObjectRenderer{
	public:
		ObjectRenderer();
		void draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection, const Camera &camera);
		unsigned int getVAO();
		Mesh* getMesh();
		void setMesh(const Mesh &mesh);
		void setMaterial(const Material &material);
		void setMaterial(unsigned int submeshIndex, const Material &material);
		void setTexture(const Texture &texture);
		void setShader(const ShaderProgram &shader);
		void reloadMesh();

	private:
		Mesh mesh;
		std::vector<unsigned int> materialIndices;
		std::vector<Material> materials;
		Texture texture;
		ShaderProgram shader;
		unsigned int VAO;
		unsigned int VBO;
		unsigned int EBO;
		void checkErrorAt(const char *location);
};

#endif