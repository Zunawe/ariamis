#ifndef CONTEXT_MANAGER_H
#define CONTEXT_MANAGER_H

#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"
#include "object_renderer.h"

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
		void display();
		void processInputs();

		static void checkErrorAt(const char *location);
		static void resizeWindow(GLFWwindow *window, int width, int height);
		static void handleMouse(GLFWwindow *window, double xPos, double yPos);

		GLFWwindow *window;

		std::vector<ObjectRenderer> objects;
		std::vector<std::shared_ptr<Light>> lights;
		Camera camera;
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;

		float cameraVelocity = 2.0;

		float deltaTime;
		float lastTime;
};

#endif
