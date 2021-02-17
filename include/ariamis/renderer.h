#ifndef RENDERER_H
#define RENDERER_H

#include <memory>

#include <glm/glm.hpp>

#include <ariamis/camera.h>
#include <ariamis/light.h>
#include <ariamis/material.h>
#include <ariamis/mesh.h>
#include <ariamis/shader.h>

class Renderer{
	public:
		Renderer();
		void draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection);
		unsigned int getVAO();
		Mesh& getMesh();
		void setMesh(const Mesh &mesh);
		Material& getMaterial();
		Material& getMaterial(unsigned int submeshIndex);
		void setMaterial(const Material &material);
		void setMaterial(unsigned int submeshIndex, const Material &material);
		void setShader(const Shader &shader);
		void reloadMesh();

	private:
		Mesh mesh;
		std::vector<unsigned int> materialIndices;
		std::vector<Material> materials;
		Shader shader;
		unsigned int VAO;
		unsigned int VBO;
		unsigned int EBO;
		void checkErrorAt(const char *location);
};

#endif
