#include "graphic.hpp"

using namespace std;

Graphic::Graphic(){
	if(!glfwInit()){
		cout << "Failed to initialize GLFW" << endl;
	}

	window = glfwCreateWindow(1024, 1024, "Graphic", NULL, NULL);
	if(window == NULL){
		cout << "Failed to create GLFW window" << endl;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(window, resizeWindow);
	glfwSetCursorPosCallback(window, handleMouse);

	glEnable(GL_DEPTH_TEST);

	ShaderProgram shader;
	Mesh cubeMesh = loadMeshFromObj("objects/cube.obj");

	Material material;
	material.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	material.diffuse = glm::vec3(0.5f, 0.0f, 0.0f);
	material.specular = glm::vec3(0.7f, 0.6f, 0.6f);
	material.shininess = 32.0f;

	shader.loadSources("shader.vs", "lighting_shader.fs");

	objects.push_back(ObjectRenderer());
	objects[0].setShader(shader);
	objects[0].setMesh(cubeMesh);
	objects[0].setMaterial(material);

	objects.push_back(ObjectRenderer());
	objects[1] = loadObj("objects/cube.obj");
	objects[1].setShader(shader);
	
	checkErrorAt("Graphic Constructor");
}

Graphic::~Graphic(){
	// Destructor
}

/**
 * Draws a single frame.
 */
void Graphic::display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	model = glm::mat4(1);
	view = glm::mat4(1);

	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

	view = camera.getViewMatrix();

	model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0, 0, 5));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0, 0, 1));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0, 1, 0));
		objects[1].draw(model, view, projection, camera);

	model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0, -1, 0));
		model = glm::scale(model, glm::vec3(10, 0.1, 10));
		objects[0].draw(model, view, projection, camera);

	glfwSwapBuffers(window);
	checkErrorAt("Display");
}

/**
 * Checks for an OpenGL error and prints the error code and location.
 * 
 * @param location a string for use in identifying where in the code the error occurs.
 */
void Graphic::checkErrorAt(const char *location){
	GLenum err;
	if((err = glGetError()) != GL_NO_ERROR){
		cout << "Error at " << location << ": " << err << endl;
	}
}

/**
 * Handles user input. Called before every frame.
 */
void Graphic::processInputs(){
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
void Graphic::run(){
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
void Graphic::resizeWindow(GLFWwindow *window, int width, int height){
	glViewport(0, 0, width, height);
}

static void handleMouse(GLFWwindow *window, double xPos, double yPos){
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

int main(){
	Graphic graphic;
	graphic.run();
	return 0;
}
