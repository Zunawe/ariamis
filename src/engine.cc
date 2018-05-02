#include "engine.h"

#include <iostream>
#include <random>

#include "shader.h"
#include "material.h"

unsigned int Engine::gBuffer;
unsigned int Engine::gPosition;
unsigned int Engine::gNormal;
unsigned int Engine::gAlbedoSpecular;
unsigned int Engine::quadVAO;
Shader Engine::lightingShader;
Shader Engine::gBufferShader;

unsigned int Engine::SSAOBuffer;
unsigned int Engine::SSAOOutput;
unsigned int Engine::SSAOIntermediate;
unsigned int Engine::rotationNoiseSSAO;
Shader Engine::SSAOShader;
Shader Engine::SSAOBlurShader;

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

	Shader::DEFAULT_SHADER.loadFile("data/shaders/forward.vs", GL_VERTEX_SHADER);
	Shader::DEFAULT_SHADER.loadFile("data/shaders/forward.fs", GL_FRAGMENT_SHADER);
	Shader::DEFAULT_SHADER.link();

	lightingShader.loadFile("data/shaders/quad.vs", GL_VERTEX_SHADER);
	lightingShader.loadFile("data/shaders/lighting.fs", GL_FRAGMENT_SHADER);
	lightingShader.link();

	gBufferShader.loadFile("data/shaders/gbuffer.vs", GL_VERTEX_SHADER);
	gBufferShader.loadFile("data/shaders/gbuffer.fs", GL_FRAGMENT_SHADER);
	gBufferShader.link();

	SSAOShader.loadFile("data/shaders/quad.vs", GL_VERTEX_SHADER);
	SSAOShader.loadFile("data/shaders/ssao.fs", GL_FRAGMENT_SHADER);
	SSAOShader.link();

	SSAOBlurShader.loadFile("data/shaders/quad.vs", GL_VERTEX_SHADER);
	SSAOBlurShader.loadFile("data/shaders/ssao_blur.fs", GL_FRAGMENT_SHADER);
	SSAOBlurShader.link();

	Material::DEFAULT_MATERIAL = Material();

	initializeGBuffer();
	initializeSSAOBuffer();

	glfwSwapInterval(0);

	std::vector<glm::vec3> kernelSSAO = generateSampleKernelSSAO(NUM_KERNEL_SAMPLES);
	for(unsigned int i = 0; i < NUM_KERNEL_SAMPLES; ++i){
		SSAOShader.setUniform("samples[" + std::to_string(i) + "]", kernelSSAO[i]);
	}
	rotationNoiseSSAO = generateRotationNoiseTextureSSAO();
}

void Engine::initializeGBuffer(){
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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
GLFWwindow* Engine::createWindow(const char *name){
	glfwDestroyWindow(window);

	if(!glfwInit()){
		std::cout << "Failed to initialize GLFW" << std::endl;
		return nullptr;
	}

	const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	width = mode->width;
	height = mode->height;

	window = glfwCreateWindow(width, height, name, glfwGetPrimaryMonitor(), NULL);

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
		std::cout << 1.0f / deltaTime << '\r' << std::flush;

		glfwPollEvents();
		processInputs();

		scene.update();

		// G-Buffer Pass
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			scene.draw();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gAlbedoSpecular);
		lightingShader.setUniform("gPosition", 0);
		lightingShader.setUniform("gNormal", 1);
		lightingShader.setUniform("gAlbedoSpecular", 2);

		// SSAO
		glBindFramebuffer(GL_FRAMEBUFFER, SSAOBuffer);
			glViewport(0, 0, width / 2.0f, height / 2.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			SSAOShader.use();
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, rotationNoiseSSAO);
			SSAOShader.setUniform("gPosition", 0);
			SSAOShader.setUniform("gNormal", 1);
			SSAOShader.setUniform("rotationNoise", 2);
			SSAOShader.setUniform("projection", scene.projection);
			drawQuad();

			SSAOBlurShader.use();
			SSAOBlurShader.setUniform("source", 3);

			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, SSAOOutput);
			SSAOBlurShader.setUniform("axis", 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, SSAOIntermediate, 0);
			drawQuad();

			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, SSAOIntermediate);
			SSAOBlurShader.setUniform("axis", 1);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, SSAOOutput, 0);
			drawQuad();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Lighting Pass
		glViewport(0, 0, width, height);
		lightingShader.use();
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, SSAOOutput);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gAlbedoSpecular);
		lightingShader.setUniform("ambientOcclusion", 3);
		lightingShader.setUniform("view", scene.cameras[0].getViewMatrix());

		for(unsigned int i = 0; i < scene.lights.size(); ++i){
			scene.lights[i]->setUniforms(lightingShader, "lights[" + std::to_string(i) + "]");
		}

		drawQuad();

		glfwSwapBuffers(window);
	}

	glfwTerminate();
}

void Engine::drawQuad(){
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
void Engine::resizeWindow(GLFWwindow * /*window*/, int newWidth, int newHeight){
	glViewport(0, 0, newWidth, newHeight);
	width = newWidth;
	height = newHeight;
	glDeleteFramebuffers(1, &gBuffer);
	initializeGBuffer();
	initializeSSAOBuffer();
}

/**
 * GLFW callback for when the mouse moves. Calls all the registered callbacks,
 * so this function can dynamically add or remove functionality.
 */
void Engine::mouseMoveCallback(GLFWwindow * /*window*/, double x, double y){
	for(auto it = mouseMoveCallbacks.begin(); it != mouseMoveCallbacks.end(); ++it){
		(*it)(x, y);
	}
}

std::vector<glm::vec3> Engine::generateSampleKernelSSAO(unsigned int numSamples){
	std::default_random_engine generator;
	std::uniform_real_distribution<float> unif(0.0f, 1.0f);
	std::vector<glm::vec3> kernel;

	for(unsigned int i = 0; i < numSamples; ++i){
		glm::vec3 sample(
			(unif(generator) * 2.0f) - 1.0f,
			(unif(generator) * 2.0f) - 1.0f,
			 unif(generator)
		);
		sample = glm::normalize(sample) * unif(generator);
		float scale = (float)i / 64.0f;
		scale = 0.1f + (scale * scale * 0.9f);
		sample *= scale;
		kernel.push_back(sample);
	}

	return kernel;
}

unsigned int Engine::generateRotationNoiseTextureSSAO(){
	std::default_random_engine generator;
	std::uniform_real_distribution<float> unif(0.0f, 1.0f);
	float rotations[16 * 3];
	for(unsigned int i = 0; i < 16 * 3; i += 3){
		rotations[i + 0] = (unif(generator) * 2.0f) - 1.0f;
		rotations[i + 1] = (unif(generator) * 2.0f) - 1.0f;
		rotations[i + 2] = 0.0f;
	}

	unsigned int id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, rotations);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	return id;
}

void Engine::initializeSSAOBuffer(){
	glDeleteFramebuffers(1, &SSAOBuffer);

	glGenFramebuffers(1, &SSAOBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, SSAOBuffer);

	glGenTextures(1, &SSAOOutput);
	glBindTexture(GL_TEXTURE_2D, SSAOOutput);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, (float)width / 2.0f, (float)height / 2.0f, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenTextures(1, &SSAOIntermediate);
	glBindTexture(GL_TEXTURE_2D, SSAOIntermediate);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, (float)width / 2.0f, (float)height / 2.0f, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, SSAOOutput, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
