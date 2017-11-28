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
	Mesh cubeMesh = createCubeMesh();

	Material material;
	material.ambient = glm::vec3(0.05375f, 0.05f, 0.06625f);
	material.diffuse = glm::vec3(0.18275f, 0.17f, 0.22525f);
	material.specular = glm::vec3(0.332741f, 0.328634f, 0.346435f);
	material.shininess = 38.4f;

	shader.loadSources("shader.vs", "lighting_shader.fs");

	cube.init();
	cube.setShader(shader);
	cube.setMesh(cubeMesh);
	cube.setMaterial(material);

	
	checkErrorAt("Graphic Constructor");
}

Graphic::~Graphic(){
	// Destructor
}

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
		cube.draw(model, view, projection, camera);

	model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0, -1, 0));
		model = glm::scale(model, glm::vec3(10, 0.1, 10));
		cube.draw(model, view, projection, camera);

	glfwSwapBuffers(window);
	checkErrorAt("Display");
}

void Graphic::checkErrorAt(const char *location){
	GLenum err;
	if((err = glGetError()) != GL_NO_ERROR){
		cout << "Error at " << location << ": " << err << endl;
	}
}

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

void Graphic::run(){
	while(!glfwWindowShouldClose(window)){
		glfwPollEvents();
		processInputs();
		display();
	}
	glfwTerminate();	
}

Mesh Graphic::createCubeMesh(){
	Mesh cube;

	// Front
	cube.addVertex(glm::vec3(-0.5, -0.5, 0.5)); cube.setNormal(glm::vec3(0, 0, 1));
	cube.addVertex(glm::vec3(0.5, -0.5, 0.5)); cube.setNormal(glm::vec3(0, 0, 1));
	cube.addVertex(glm::vec3(0.5, 0.5, 0.5)); cube.setNormal(glm::vec3(0, 0, 1));
	cube.addVertex(glm::vec3(-0.5, 0.5, 0.5)); cube.setNormal(glm::vec3(0, 0, 1));
	cube.addTriangle(0, 1, 2);
	cube.addTriangle(0, 2, 3);
	cube.setTextureCoordinate(0, glm::vec2(0.0f, 0.0f));
	cube.setTextureCoordinate(1, glm::vec2(1.0f, 0.0f));
	cube.setTextureCoordinate(2, glm::vec2(1.0f, 1.0f));
	cube.setTextureCoordinate(3, glm::vec2(0.0f, 1.0f));
	
	// Back
	cube.addVertex(glm::vec3(0.5, -0.5, -0.5)); cube.setNormal(glm::vec3(0, 0, -1));
	cube.addVertex(glm::vec3(-0.5, -0.5, -0.5)); cube.setNormal(glm::vec3(0, 0, -1));
	cube.addVertex(glm::vec3(-0.5, 0.5, -0.5)); cube.setNormal(glm::vec3(0, 0, -1));
	cube.addVertex(glm::vec3(0.5, 0.5, -0.5)); cube.setNormal(glm::vec3(0, 0, -1));
	cube.addTriangle(4, 5, 6);
	cube.addTriangle(4, 6, 7);
	cube.setTextureCoordinate(4, glm::vec2(0.0f, 0.0f));
	cube.setTextureCoordinate(5, glm::vec2(1.0f, 0.0f));
	cube.setTextureCoordinate(6, glm::vec2(1.0f, 1.0f));
	cube.setTextureCoordinate(7, glm::vec2(0.0f, 1.0f));

	// Left
	cube.addVertex(glm::vec3(-0.5, -0.5, -0.5)); cube.setNormal(glm::vec3(-1, 0, 0));
	cube.addVertex(glm::vec3(-0.5, -0.5, 0.5)); cube.setNormal(glm::vec3(-1, 0, 0));
	cube.addVertex(glm::vec3(-0.5, 0.5, 0.5)); cube.setNormal(glm::vec3(-1, 0, 0));
	cube.addVertex(glm::vec3(-0.5, 0.5, -0.5)); cube.setNormal(glm::vec3(-1, 0, 0));
	cube.addTriangle(8, 9, 10);
	cube.addTriangle(8, 10, 11);
	cube.setTextureCoordinate(8, glm::vec2(0.0f, 0.0f));
	cube.setTextureCoordinate(9, glm::vec2(1.0f, 0.0f));
	cube.setTextureCoordinate(10, glm::vec2(1.0f, 1.0f));
	cube.setTextureCoordinate(11, glm::vec2(0.0f, 1.0f));

	// Right
	cube.addVertex(glm::vec3(0.5, -0.5, 0.5)); cube.setNormal(glm::vec3(1, 0, 0));
	cube.addVertex(glm::vec3(0.5, -0.5, -0.5)); cube.setNormal(glm::vec3(1, 0, 0));
	cube.addVertex(glm::vec3(0.5, 0.5, -0.5)); cube.setNormal(glm::vec3(1, 0, 0));
	cube.addVertex(glm::vec3(0.5, 0.5, 0.5)); cube.setNormal(glm::vec3(1, 0, 0));
	cube.addTriangle(12, 13, 14);
	cube.addTriangle(12, 14, 15);
	cube.setTextureCoordinate(12, glm::vec2(0.0f, 0.0f));
	cube.setTextureCoordinate(13, glm::vec2(1.0f, 0.0f));
	cube.setTextureCoordinate(14, glm::vec2(1.0f, 1.0f));
	cube.setTextureCoordinate(15, glm::vec2(0.0f, 1.0f));

	// Top
	cube.addVertex(glm::vec3(-0.5, 0.5, 0.5)); cube.setNormal(glm::vec3(0, 1, 0));
	cube.addVertex(glm::vec3(0.5, 0.5, 0.5)); cube.setNormal(glm::vec3(0, 1, 0));
	cube.addVertex(glm::vec3(0.5, 0.5, -0.5)); cube.setNormal(glm::vec3(0, 1, 0));
	cube.addVertex(glm::vec3(-0.5, 0.5, -0.5)); cube.setNormal(glm::vec3(0, 1, 0));
	cube.addTriangle(16, 17, 18);
	cube.addTriangle(16, 18, 19);
	cube.setTextureCoordinate(16, glm::vec2(0.0f, 0.0f));
	cube.setTextureCoordinate(17, glm::vec2(1.0f, 0.0f));
	cube.setTextureCoordinate(18, glm::vec2(1.0f, 1.0f));
	cube.setTextureCoordinate(19, glm::vec2(0.0f, 1.0f));

	// Bottom
	cube.addVertex(glm::vec3(0.5, -0.5, 0.5)); cube.setNormal(glm::vec3(0, -1, 0));
	cube.addVertex(glm::vec3(-0.5, -0.5, 0.5)); cube.setNormal(glm::vec3(0, -1, 0));
	cube.addVertex(glm::vec3(-0.5, -0.5, -0.5)); cube.setNormal(glm::vec3(0, -1, 0));
	cube.addVertex(glm::vec3(0.5, -0.5, -0.5)); cube.setNormal(glm::vec3(0, -1, 0));
	cube.addTriangle(20, 21, 22);
	cube.addTriangle(20, 22, 23);
	cube.setTextureCoordinate(20, glm::vec2(0.0f, 0.0f));
	cube.setTextureCoordinate(21, glm::vec2(1.0f, 0.0f));
	cube.setTextureCoordinate(22, glm::vec2(1.0f, 1.0f));
	cube.setTextureCoordinate(23, glm::vec2(0.0f, 1.0f));

	return cube;
}

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
