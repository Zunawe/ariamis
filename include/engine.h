#ifndef ENGINE_H
#define ENGINE_H

#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

#include "scene.h"

class Engine{
	public:
		static Engine* getInstance();

		~Engine();
		GLFWwindow* createWindow(int width, int height, const char *name);
		void playScene(Scene scene);
		void processInputs();
		GLFWwindow* getWindow();
		int getWidth();
		int getHeight();

	private:
		static void resizeWindow(GLFWwindow *window, int newWidth, int newHeight);

		Engine();
		void postContextCreation();

		static Engine *instance;
		float lastTime;
		float deltaTime;
		int width;
		int height;
		GLFWwindow* window;
};

#endif
