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

void Engine::postContextCreation(){
	glfwSetFramebufferSizeCallback(window, resizeWindow);
	glfwSetCursorPosCallback(window, mouseMoveCallback);
	glEnable(GL_DEPTH_TEST);
	Shader::DEFAULT_SHADER.loadSources("data/shaders/default.vs", "data/shaders/default.fs");
	Material::DEFAULT_MATERIAL = Material();
}

void Engine::cleanUp(){
	glfwDestroyWindow(window);
	keyCallbacks.clear();
	mouseMoveCallbacks.clear();
}

GLFWwindow* Engine::createWindow(int width, int height, const char *name){
	glfwDestroyWindow(window);

	if(!glfwInit()){
		std::cout << "Failed to initialize GLFW" << std::endl;
		return nullptr;
	}

	window = glfwCreateWindow(width, height, name, NULL, NULL);
	width = width;
	height = height;

	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	postContextCreation();

	return window;
}

void Engine::playScene(Scene &scene){
	while(!glfwWindowShouldClose(window)){
		float newTime = glfwGetTime();
		deltaTime = newTime - lastTime;
		lastTime = newTime;

		glfwPollEvents();
		processInputs();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		scene.draw();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
}

void Engine::processInputs(){
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		glfwSetWindowShouldClose(window, true);
	}
	for(auto it = keyCallbacks.begin(); it != keyCallbacks.end(); ++it){
		if(glfwGetKey(window, it->first) == GLFW_PRESS){
			for(auto fp = it->second.begin(); fp != it->second.end(); ++fp){
				(*fp)(deltaTime);
			}
		}
	}
}

void Engine::registerKeyEvent(int key, std::function<void(float)> func){
	keyCallbacks[key].push_back(func);
}

void Engine::registerMouseMoveEvent(std::function<void(double, double)> func){
	mouseMoveCallbacks.push_back(func);
}

GLFWwindow* Engine::getWindow(){
	return window;
}

int Engine::getWidth(){
	return width;
}

int Engine::getHeight(){
	return height;
}

void Engine::resizeWindow(GLFWwindow *window, int newWidth, int newHeight){
	glViewport(0, 0, newWidth, newHeight);
	width = newWidth;
	height = newHeight;
}

void Engine::mouseMoveCallback(GLFWwindow *window, double x, double y){
	auto callbacks = mouseMoveCallbacks;

	for(auto it = callbacks.begin(); it != callbacks.end(); ++it){
		(*it)(x, y);
	}
}
