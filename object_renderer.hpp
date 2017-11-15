#ifndef OPENGL_HEADERS
#define OPENGL_HEADERS
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#endif

#include "mesh.hpp"
#include "shader_program.hpp"
#include "texture.hpp"

class ObjectRenderer{
	public:
		ObjectRenderer();
		void draw(const glm::mat4 *model, const glm::mat4 *view, const glm::mat4 *projection);
		unsigned int getVAO();
		void setMesh(Mesh mesh);
		void setTexture(Texture texture);
		void setShader(ShaderProgram shader);

	private:
		Mesh mesh;
		Texture texture;
		ShaderProgram shader;
		unsigned int VAO;
		unsigned int VBO;
		unsigned int EBO;
		void checkErrorAt(const char *location);
};