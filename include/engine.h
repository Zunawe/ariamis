#ifndef ENGINE_H
#define ENGINE_H

#include <map>
#include <vector>
#include <functional>

#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

#include "scene.h"

#define NUM_KERNEL_SAMPLES 32

class Engine{
	public:
		static void playScene(Scene &scene);
		static void registerKeyEvent(int key, std::function<void(float)> func);
		static void registerMouseMoveEvent(std::function<void(double, double)> func);
		static GLFWwindow* createWindow(const char *name);
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
		static void initializeGBuffer();
		static void initializeSSAOBuffer();
		static void processInputs();
		static void drawQuad();
		static std::vector<glm::vec3> generateSampleKernelSSAO(unsigned int numSamples);
		static unsigned int generateRotationNoiseTextureSSAO();

		static unsigned int gBuffer;
		static unsigned int gPosition;
		static unsigned int gNormal;
		static unsigned int gAlbedoSpecular;
		static unsigned int quadVAO;
		static Shader lightingShader;
		static Shader gBufferShader;

		static unsigned int SSAOBuffer;
		static unsigned int SSAOOutput;
		static unsigned int SSAOIntermediate;
		static std::vector<glm::vec3> kernelSSAO;
		static unsigned int rotationNoiseSSAO;
		static Shader SSAOShader;
		static Shader SSAOBlurShader;

		static std::map<int, std::vector<std::function<void(float)>>> keyCallbacks;
		static std::vector<std::function<void(double, double)>> mouseMoveCallbacks;
		static float lastTime;
		static float deltaTime;
		static int width;
		static int height;
		static GLFWwindow *window;
};

#endif
