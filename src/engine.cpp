#include "engine.h"

#include <iostream>

#include "shader.h"
#include "material.h"

std::map<int, std::vector<std::function<void(float)>>> Engine::keyCallbacks;
std::vector<std::function<void(double, double)>> Engine::mouseMoveCallbacks;
float Engine::lastTime;
float Engine::deltaTime;
int Engine::width;
int Engine::height;
GLFWwindow *Engine::window;

/**
 * Initialization that should happen after a GL context has been created.
 */
void Engine::postContextCreation(){
	glfwSetFramebufferSizeCallback(window, resizeWindow);
	glfwSetCursorPosCallback(window, mouseMoveCallback);
	glEnable(GL_DEPTH_TEST);
	Shader::DEFAULT_SHADER.loadSources("data/shaders/default.vs", "data/shaders/default.fs");
	Material::DEFAULT_MATERIAL = Material();
}

/**
 * Reset Engine for use again during this runtime if necessary.
 */
void Engine::cleanUp(){
	glfwDestroyWindow(window);
	keyCallbacks.clear();
	mouseMoveCallbacks.clear();
}

/**
 * Create the GLFW window that will contain the GL context and initialize
 * things that require said context. You should do this before creating any
 * other instances of anything from this library.
 */
GLFWwindow* Engine::createWindow(int width, int height, const char *name){
	glfwDestroyWindow(window);

	if(!glfwInit()){
		std::cout << "Failed to initialize GLFW" << std::endl;
		return nullptr;
	}

	window = glfwCreateWindow(width, height, name, NULL, NULL);
	Engine::width = width;
	Engine::height = height;

	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	postContextCreation();

	return window;
}

/**
 * Run a scene until the window is closed. This will start the main loop,
 * poll input events, and draw frames.
 * 
 * @param scene the scene to display and run.
 */
void Engine::playScene(Scene &scene){
	while(!glfwWindowShouldClose(window)){
		float newTime = glfwGetTime();
		deltaTime = newTime - lastTime;
		lastTime = newTime;

		glfwPollEvents();
		processInputs();

		scene.update();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		scene.draw();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
}

/**
 * Loops through all the callbacks for keypresses and calls the ones that have
 * a key pressed.
 */
void Engine::processInputs(){
	for(auto it = keyCallbacks.begin(); it != keyCallbacks.end(); ++it){
		if(glfwGetKey(window, it->first) == GLFW_PRESS){
			for(auto fp = it->second.begin(); fp != it->second.end(); ++fp){
				(*fp)(deltaTime);
			}
		}
	}
}

/**
 * Closes the window after the current frame finishes rendering.
 */
void Engine::quit(){
	glfwSetWindowShouldClose(window, true);
}

/**
 * Registers a new key callback. This function will be called every frame key
 * is being pressed.
 * 
 * @param key a GLFW key code (e.g. GLFW_KEY_W)
 * @param func the function to run while the key is pressed. It will be passed
 * the time since the last frame.
 */
void Engine::registerKeyEvent(int key, std::function<void(float)> func){
	keyCallbacks[key].push_back(func);
}

/**
 * Registers a new mouse movement callback. This function will be called every
 * time the mouse position changes.
 * 
 * @param func the function to run when the mouse moves. It will be passed
 * the screen space x and y coordinates.
 */
void Engine::registerMouseMoveEvent(std::function<void(double, double)> func){
	mouseMoveCallbacks.push_back(func);
}

/**
 * Gets the GLFW window pointer.
 * 
 * @return the GLFW window pointer.
 */
GLFWwindow* Engine::getWindow(){
	return window;
}

int Engine::getWidth(){
	return width;
}

int Engine::getHeight(){
	return height;
}

float Engine::getDeltaTime(){
	return deltaTime;
}

float Engine::getTime(){
	return lastTime;
}

/**
 * GLFW callback for when the window is resized. Resets the viewport and records
 * the new width and height.
 */
void Engine::resizeWindow(GLFWwindow *window, int newWidth, int newHeight){
	glViewport(0, 0, newWidth, newHeight);
	width = newWidth;
	height = newHeight;
}

/**
 * GLFW callback for when the mouse moves. Calls all the registered callbacks,
 * so this function can dynamically add or remove functionality.
 */
void Engine::mouseMoveCallback(GLFWwindow *window, double x, double y){
	for(auto it = mouseMoveCallbacks.begin(); it != mouseMoveCallbacks.end(); ++it){
		(*it)(x, y);
	}
}
