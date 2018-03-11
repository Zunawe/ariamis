#ifndef ENGINE_H
#define ENGINE_H

#include <map>
#include <vector>
#include <functional>

#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

#include "scene.h"

class Engine{
	public:
		static void playScene(Scene &scene);
		static void registerKeyEvent(int key, std::function<void(float)> func);
		static void registerMouseMoveEvent(std::function<void(double, double)> func);
		static GLFWwindow* createWindow(int width, int height, const char *name);
		static GLFWwindow* getWindow();
		static void quit();
		static int getWidth();
		static int getHeight();
		static float getDeltaTime();
		static float getTime();
		static void cleanUp();

	private:
		static void resizeWindow(GLFWwindow *window, int newWidth, int newHeight);
		static void mouseMoveCallback(GLFWwindow *window, double x, double y);

		static void postContextCreation();
		static void processInputs();

		static std::map<int, std::vector<std::function<void(float)>>> keyCallbacks;
		static std::vector<std::function<void(double, double)>> mouseMoveCallbacks;
		static float lastTime;
		static float deltaTime;
		static int width;
		static int height;
		static GLFWwindow *window;
};

#endif
