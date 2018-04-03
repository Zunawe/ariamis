#include "engine.h"

#include <iostream>

#include "shader.h"
#include "material.h"

unsigned int Engine::gBuffer;
unsigned int Engine::gPosition;
unsigned int Engine::gNormal;
unsigned int Engine::gAlbedoSpecular;
unsigned int Engine::quadVAO;
Shader Engine::lightingShader;
Shader Engine::gBufferShader;

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

	Shader::DEFAULT_SHADER.loadFile("data/shaders/default.vs", GL_VERTEX_SHADER);
	Shader::DEFAULT_SHADER.loadFile("data/shaders/default.fs", GL_FRAGMENT_SHADER);
	Shader::DEFAULT_SHADER.link();

	lightingShader.loadFile("data/shaders/lighting.vs", GL_VERTEX_SHADER);
	lightingShader.loadFile("data/shaders/lighting.fs", GL_FRAGMENT_SHADER);
	lightingShader.link();

	gBufferShader.loadFile("data/shaders/gbuffer.vs", GL_VERTEX_SHADER);
	gBufferShader.loadFile("data/shaders/gbuffer.fs", GL_FRAGMENT_SHADER);
	gBufferShader.link();

	Material::DEFAULT_MATERIAL = Material();
	initializeGBuffer();
}

void Engine::initializeGBuffer(){
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	glGenTextures(1, &gAlbedoSpecular);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpecular);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpecular, 0);

	unsigned int attachments[]{GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
	glDrawBuffers(3, attachments);

	unsigned int depthRBO;
	glGenRenderbuffers(1, &depthRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
	for(auto it = scene.objects.begin(); it != scene.objects.end(); ++it){
		(*it)->renderer.setShader(gBufferShader);
	}

	while(!glfwWindowShouldClose(window)){
		float newTime = glfwGetTime();
		deltaTime = newTime - lastTime;
		lastTime = newTime;

		glfwPollEvents();
		processInputs();

		scene.update();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			scene.draw();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		renderQuad(scene.lights, scene.cameras[0].getPosition());
		glfwSwapBuffers(window);
	}

	glfwTerminate();
}

void Engine::renderQuad(const std::vector<std::shared_ptr<Light>> &lights, glm::vec3 cameraPosition){
	if(!quadVAO){
		unsigned int quadVBO;
		float attributes[]{
			 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
			-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
			-1.0f,  1.0f, 0.0f,  0.0f, 1.0f
		};
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);

		glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(attributes), &attributes, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
		glBindVertexArray(0);
	}

	lightingShader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpecular);
	lightingShader.setUniform("gPosition", 0);
	lightingShader.setUniform("gNormal", 1);
	lightingShader.setUniform("gAlbedoSpecular", 2);
	lightingShader.setUniform("cameraPosition", cameraPosition);

	for(int i = 0; i < lights.size(); ++i){
		lights[i]->setUniforms(lightingShader, "lights[" + std::to_string(i) + "]");
	}

	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
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
	glDeleteFramebuffers(1, &gBuffer);
	initializeGBuffer();
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
