#include "engine.h"

#include <iostream>

#include "shader.h"
#include "material.h"

Engine Engine::instance;

Engine::Engine(){
	// Empty
}

void Engine::postContextCreation(){
	glEnable(GL_DEPTH_TEST);
	Shader::DEFAULT_SHADER.loadSources("data/shaders/default.vs", "data/shaders/default.fs");
	Material::DEFAULT_MATERIAL = Material();
}

Engine::~Engine(){
	glfwDestroyWindow(window);
}

Engine Engine::getInstance(){
	return Engine::instance;
}

GLFWwindow* Engine::createWindow(int width, int height, const char *name){
	glfwDestroyWindow(window);

	if(!glfwInit()){
		std::cout << "Failed to initialize GLFW" << std::endl;
		return nullptr;
	}

	this->window = glfwCreateWindow(width, height, name, NULL, NULL);
	this->width = width;
	this->height = height;

	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	postContextCreation();

	return window;
}

void Engine::playScene(Scene scene){
	while(!glfwWindowShouldClose(window)){
		glfwPollEvents();
		// processInputs();
		scene.draw();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
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
