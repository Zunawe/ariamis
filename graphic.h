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
#endif

#include "shader_program.h"
#include "mesh.h"
#include "texture.h"
#include "model_view_matrix.h"

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
		ModelViewMatrix *model;
		
		unsigned int VBO;
		unsigned int EBO;
		unsigned int VAO;

		void display();
		void checkErrorAt(const char *location);
		static void resizeWindow(GLFWwindow *window, int width, int height);
		Mesh* createCubeMesh();
		void processInputs();
};