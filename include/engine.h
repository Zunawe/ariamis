#ifndef ENGINE_H
#define ENGINE_H

#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

#include "scene.h"

class Engine{
	public:
		static Engine getInstance();

		~Engine();
		GLFWwindow* createWindow(int width, int height, const char *name);
		void playScene(Scene scene);
		GLFWwindow* getWindow();
		int getWidth();
		int getHeight();

	private:
		Engine();
		void postContextCreation();

		static Engine instance;
		int width;
		int height;
		GLFWwindow* window;
};

#endif
