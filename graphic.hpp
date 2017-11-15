#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <time.h>

#ifndef OPENGL_HEADERS
#define OPENGL_HEADERS
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#endif

#include "shader_program.hpp"
#include "mesh.hpp"
#include "texture.hpp"
#include "camera.hpp"

using namespace std;

class Graphic{
	public:
		Graphic();
		void run();
	
	private:
		GLFWwindow *window;		
		ShaderProgram shaderProgram;
		Mesh *mesh;
		Texture *texture;
		Camera camera;
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;

		int theta;
		int phi;
		
		unsigned int VBO;
		unsigned int EBO;
		unsigned int VAO;

		void display();
		void checkErrorAt(const char *location);
		static void resizeWindow(GLFWwindow *window, int width, int height);
		Mesh* createCubeMesh();
		void processInputs();
};