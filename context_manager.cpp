#include "context_manager.hpp"

using namespace std;

float ContextManager::sensitivity = 0.1f;
glm::vec3 ContextManager::mouseLook = glm::vec3(0, 0, 1);
float ContextManager::lastMouseX;
float ContextManager::lastMouseY;
float ContextManager::pitch;
float ContextManager::yaw;
int ContextManager::width;
int ContextManager::height;

ContextManager::ContextManager(GLFWwindow *window){
	this->window = window;
	glfwSetFramebufferSizeCallback(window, resizeWindow);
	glfwSetCursorPosCallback(window, handleMouse);

	Shader::DEFAULT_SHADER.loadSources("shaders/default.vs", "shaders/default.fs");
	Material::DEFAULT_MATERIAL = Material();

	objects.push_back(loadObj("objects/cube.obj"));
	objects.push_back(loadObj("objects/sphere.obj"));

	shared_ptr<PointLight> pointLight(new PointLight());
	pointLight->position = glm::vec4(1, 0, 5, 1);
	pointLight->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	pointLight->specular = glm::vec3(1.0f, 1.0f, 1.0f);
	pointLight->kc = 1.0f;
	pointLight->kl = 0.05f;
	pointLight->kq = 0.001f;
	lights.push_back(pointLight);

	shared_ptr<Light> directionalLight(new Light());
	directionalLight->position = glm::vec4(-1, -1, 0, 0); // Direction
	directionalLight->ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	directionalLight->diffuse = glm::vec3(0.7f, 0.7f, 0.7f);
	directionalLight->specular = glm::vec3(0.9f, 0.9f, 0.9f);
	lights.push_back(directionalLight);

	camera.setPosition(glm::vec3(0, 0, -5));
	
	checkErrorAt("ContextManager Constructor");
}

/**
 * Draws a single frame.
 */
void ContextManager::display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	model = glm::mat4(1);
	view = camera.getViewMatrix();
	projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

	model = glm::mat4(1);
		objects[1].draw(model, view, projection, camera, lights);

	model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0, -1.5, 0));
		model = glm::scale(model, glm::vec3(50, 0.1, 50));
		objects[0].draw(model, view, projection, camera, lights);

	glfwSwapBuffers(window);
	checkErrorAt("Display");
}

/**
 * Checks for an OpenGL error and prints the error code and location.
 * 
 * @param location a string for use in identifying where in the code the error occurs.
 */
void ContextManager::checkErrorAt(const char *location){
	GLenum err;
	if((err = glGetError()) != GL_NO_ERROR){
		cout << "Error at " << location << ": " << err << endl;
	}
}

/**
 * Handles user input. Called before every frame.
 */
void ContextManager::processInputs(){
	float newTime = glfwGetTime();
	deltaTime = newTime - lastTime;
	lastTime = newTime;

	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		glfwSetWindowShouldClose(window, true);
	}
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
		camera.move(glm::cross(camera.getUp(), camera.getRight()), cameraVelocity * deltaTime);
	}
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
		camera.move(glm::cross(camera.getRight(), camera.getUp()), cameraVelocity * deltaTime);
	}
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
		camera.move(-camera.getRight(), cameraVelocity * deltaTime);
	}
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
		camera.move(camera.getRight(), cameraVelocity * deltaTime);
	}
	if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
		camera.move(glm::vec3(0, 1, 0), cameraVelocity * deltaTime);
	}
	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
		camera.move(glm::vec3(0, -1, 0), cameraVelocity * deltaTime);
	}
	camera.setForward(mouseLook);
}

/**
 * Main loop.
 */
void ContextManager::run(){
	while(!glfwWindowShouldClose(window)){
		glfwPollEvents();
		processInputs();
		display();
	}
	glfwTerminate();	
}

/**
 * Callback for GLFW defining the behavior for resizing a window.
 */
void ContextManager::resizeWindow(GLFWwindow *window, int newWidth, int newHeight){
	glViewport(0, 0, newWidth, newHeight);
	width = newWidth;
	height = newHeight;
}

/**
 * Callback for GLFW defining the behavior for mouse movement.
 */
void ContextManager::handleMouse(GLFWwindow *window, double xPos, double yPos){
	float deltaX = (xPos - lastMouseX) * sensitivity;
	float deltaY = -(yPos - lastMouseY) * sensitivity;

	lastMouseX = xPos;
	lastMouseY = yPos;

	yaw += deltaX;
	pitch += deltaY;

	pitch = pitch > 89.0f ? 89.0f : (pitch < -89.0f ? -89.0f : pitch);
	yaw = yaw > 360.0f ? yaw - 360.0f : (yaw < 0.0f ? yaw + 360.0f : yaw);

	mouseLook.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	mouseLook.y = sin(glm::radians(pitch));
	mouseLook.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	mouseLook = glm::normalize(mouseLook);
}
