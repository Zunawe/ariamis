#ifndef OBJECT_RENDERER_H
#define OBJECT_RENDERER_H

#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

#include "mesh.hpp"
#include "shader_program.hpp"
#include "texture.hpp"
#include "camera.hpp"

struct Material{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

struct Light{
	glm::vec3 pos;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

class ObjectRenderer{
	public:
		void init();
		void draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection, const Camera &camera);
		unsigned int getVAO();
		void setMesh(const Mesh &mesh);
		void setMaterial(const Material &material);
		void setTexture(const Texture &texture);
		void setShader(const ShaderProgram &shader);

	private:
		Mesh mesh;
		Material material;
		Texture texture;
		ShaderProgram shader;
		unsigned int VAO;
		unsigned int VBO;
		unsigned int EBO;
		void checkErrorAt(const char *location);
};

#endif