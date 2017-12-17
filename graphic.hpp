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
#include "obj_loader.hpp"

static void handleMouse(GLFWwindow *window, double xPos, double yPos);

float lastMouseX;
float lastMouseY;
float sensitivity = 0.1f;
float pitch;
float yaw;

glm::vec3 mouseLook(0, 0, 1);

class Graphic{
	public:
		Graphic();
		~Graphic();
		void run();
	
	private:
		GLFWwindow *window;
		ObjectRenderer cube;
		ObjectRenderer sword;
		ObjectRenderer lamp;
		Camera camera;
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;

		float cameraVelocity = 2;

		float deltaTime;
		float lastTime;

		void display();
		static void checkErrorAt(const char *location);
		static void resizeWindow(GLFWwindow *window, int width, int height);
		static Mesh createCubeMesh();
		void processInputs();
};

#endif