#ifndef CONTEXT_MANAGER_H
#define CONTEXT_MANAGER_H

#include <time.h>

#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <memory>

#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"
#include "camera.hpp"
#include "texture.hpp"
#include "object_renderer.hpp"
#include "obj_loader.hpp"

static void handleMouse(GLFWwindow *window, double xPos, double yPos);

class ContextManager{
	public:
		ContextManager(GLFWwindow *window);
		void run();

		static float lastMouseX;
		static float lastMouseY;
		static float sensitivity;
		static float pitch;
		static float yaw;

		static glm::vec3 mouseLook;
		static int width;
		static int height;
	
	private:
		GLFWwindow *window;

		std::vector<ObjectRenderer> objects;
		std::vector<std::shared_ptr<Light>> lights;
		ObjectRenderer lamp;
		Camera camera;
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;

		float cameraVelocity = 2.0;

		float deltaTime;
		float lastTime;

		void display();
		static void checkErrorAt(const char *location);
		static void resizeWindow(GLFWwindow *window, int width, int height);
		static void handleMouse(GLFWwindow *window, double xPos, double yPos);
		void processInputs();
};

#endif
