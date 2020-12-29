#ifndef ENGINE_H
#define ENGINE_H

#include <functional>
#include <map>
#include <vector>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

class Scene;
class Shader;

class Engine{
	public:
		static GLFWwindow* createWindow(const char *name);
		static GLFWwindow* getWindow();
		static void registerKeyEvent(int key, std::function<void(float)> func);
		static void registerMouseMoveEvent(std::function<void(double, double)> func);
		static void playScene(Scene &scene);
		static int getWidth();
		static int getHeight();
		static float getTime();
		static float getDeltaTime();
		static void cleanUp();
		static void quit();

	private:
		static void resizeWindow(GLFWwindow * /*window*/, int newWidth, int newHeight);
		static void mouseMoveCallback(GLFWwindow * /*window*/, double x, double y);

		static void postContextCreation();
		static void initializeGBuffer();
		static void processInputs();
		static void drawQuad();

		static unsigned int gBuffer;
		static unsigned int gPosition;
		static unsigned int gNormal;
		static unsigned int gAlbedoSpecular;
		static Shader lightingShader;
		static Shader gBufferShader;

		static unsigned int quadVAO;

		static std::map<int, std::vector<std::function<void(float)>>> keyCallbacks;
		static std::vector<std::function<void(double, double)>> mouseMoveCallbacks;
		static float lastTime;
		static float deltaTime;
		static unsigned int width;
		static unsigned int height;
		static GLFWwindow *window;
};

#endif
