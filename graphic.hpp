#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <time.h>

#ifndef OPENGL_HEADERS
#define OPENGL_HEADERS
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.hpp"
#include "object_renderer.hpp"

using namespace std;

class Graphic{
	public:
		Graphic();
		void run();
	
	private:
		GLFWwindow *window;
		ObjectRenderer *cube;
		Camera camera;
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;

		int theta;
		int phi;

		void display();
		void checkErrorAt(const char *location);
		static void resizeWindow(GLFWwindow *window, int width, int height);
		Mesh* createCubeMesh();
		void processInputs();
};