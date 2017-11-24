#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <time.h>

#include <iostream>
#include <string>
#include <cmath>
#include <vector>

#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.hpp"
#include "texture.hpp"
#include "object_renderer.hpp"

class Graphic{
	public:
		Graphic();
		~Graphic();
		void run();
	
	private:
		GLFWwindow *window;
		ObjectRenderer texturedCube;
		ObjectRenderer coloredCube;
		ObjectRenderer lamp;
		Camera camera;
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;

		int theta;
		int phi;

		void display();
		void checkErrorAt(const char *location);
		static void resizeWindow(GLFWwindow *window, int width, int height);
		Mesh createCubeMesh();
		void processInputs();
};

#endif