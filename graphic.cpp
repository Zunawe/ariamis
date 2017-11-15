#include "graphic.hpp"

Graphic::Graphic(){
	if(!glfwInit()){
		cout << "Failed to initialize GLFW" << endl;
	}

	window = glfwCreateWindow(1024, 1024, "Graphic", NULL, NULL);
	if(window == NULL){
		cout << "Failed to create GLFW window" << endl;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, resizeWindow);

	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_DEPTH_TEST);

	ShaderProgram shaderProgram;
	Texture texture;

	shaderProgram.loadSources("vertex_shader.glsl", "fragment_shader.glsl");
	texture.load("texture.png");

	cube = new ObjectRenderer();
	cube->setShader(shaderProgram);
	cube->setMesh(createCubeMesh());
	cube->setTexture(texture);

	theta = 0.0f;
	phi = 90.0f;
	
	checkErrorAt("Graphic Constructor");
}

Graphic::~Graphic(){
	delete cube;
}

void Graphic::display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	model = glm::mat4(1);
	view = glm::mat4(1);

	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

	view = glm::translate(view, glm::vec3(0, 0, -3));
	view = glm::rotate(view, glm::radians((float)phi + 90), glm::vec3(1, 0, 0));
	view = glm::rotate(view, glm::radians((float)theta), glm::vec3(0, 1, 0));

	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0, 0, 1));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0, 1, 0));

	cube->draw(&model, &view, &projection);

	glfwSwapBuffers(window);
}

void Graphic::checkErrorAt(const char *location){
	GLenum err;
	if((err = glGetError()) != GL_NO_ERROR){
		cout << "Error at " << location << ": " << (err == GL_INVALID_VALUE) << endl;
	}
}

void Graphic::processInputs(){
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		glfwSetWindowShouldClose(window, true);
	}
	if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
		phi += 3;
		if(phi > 180){
			phi = 180;
		}
	}
	if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
		phi -= 3;
		if(phi < 0){
			phi = 0;
		}
	}
	if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
		theta -= 3;
	}
	if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
		theta += 3;
	}
	theta = theta % 360;
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

	// cube->setDefaultColor(glm::vec3(1.0f, 0.0f, 1.0f));

	// Front
	cube.addVertex(glm::vec3(-0.5, -0.5, 0.5));
	cube.addVertex(glm::vec3(0.5, -0.5, 0.5));
	cube.addVertex(glm::vec3(0.5, 0.5, 0.5));
	cube.addVertex(glm::vec3(-0.5, 0.5, 0.5));
	cube.addTriangle(0, 1, 2);
	cube.addTriangle(0, 2, 3);
	cube.setTextureCoordinate(0, glm::vec2(0.0f, 0.0f));
	cube.setTextureCoordinate(1, glm::vec2(1.0f, 0.0f));
	cube.setTextureCoordinate(2, glm::vec2(1.0f, 1.0f));
	cube.setTextureCoordinate(3, glm::vec2(0.0f, 1.0f));
	
	// Back
	cube.addVertex(glm::vec3(0.5, -0.5, -0.5));
	cube.addVertex(glm::vec3(-0.5, -0.5, -0.5));
	cube.addVertex(glm::vec3(-0.5, 0.5, -0.5));
	cube.addVertex(glm::vec3(0.5, 0.5, -0.5));
	cube.addTriangle(4, 5, 6);
	cube.addTriangle(4, 6, 7);
	cube.setTextureCoordinate(4, glm::vec2(0.0f, 0.0f));
	cube.setTextureCoordinate(5, glm::vec2(1.0f, 0.0f));
	cube.setTextureCoordinate(6, glm::vec2(1.0f, 1.0f));
	cube.setTextureCoordinate(7, glm::vec2(0.0f, 1.0f));

	// Left
	cube.addVertex(glm::vec3(-0.5, -0.5, -0.5));
	cube.addVertex(glm::vec3(-0.5, -0.5, 0.5));
	cube.addVertex(glm::vec3(-0.5, 0.5, 0.5));
	cube.addVertex(glm::vec3(-0.5, 0.5, -0.5));
	cube.addTriangle(8, 9, 10);
	cube.addTriangle(8, 10, 11);
	cube.setTextureCoordinate(8, glm::vec2(0.0f, 0.0f));
	cube.setTextureCoordinate(9, glm::vec2(1.0f, 0.0f));
	cube.setTextureCoordinate(10, glm::vec2(1.0f, 1.0f));
	cube.setTextureCoordinate(11, glm::vec2(0.0f, 1.0f));

	// Right
	cube.addVertex(glm::vec3(0.5, -0.5, 0.5));
	cube.addVertex(glm::vec3(0.5, -0.5, -0.5));
	cube.addVertex(glm::vec3(0.5, 0.5, -0.5));
	cube.addVertex(glm::vec3(0.5, 0.5, 0.5));
	cube.addTriangle(12, 13, 14);
	cube.addTriangle(12, 14, 15);
	cube.setTextureCoordinate(12, glm::vec2(0.0f, 0.0f));
	cube.setTextureCoordinate(13, glm::vec2(1.0f, 0.0f));
	cube.setTextureCoordinate(14, glm::vec2(1.0f, 1.0f));
	cube.setTextureCoordinate(15, glm::vec2(0.0f, 1.0f));

	// Top
	cube.addVertex(glm::vec3(-0.5, 0.5, 0.5));
	cube.addVertex(glm::vec3(0.5, 0.5, 0.5));
	cube.addVertex(glm::vec3(0.5, 0.5, -0.5));
	cube.addVertex(glm::vec3(-0.5, 0.5, -0.5));
	cube.addTriangle(16, 17, 18);
	cube.addTriangle(16, 18, 19);
	cube.setTextureCoordinate(16, glm::vec2(0.0f, 0.0f));
	cube.setTextureCoordinate(17, glm::vec2(1.0f, 0.0f));
	cube.setTextureCoordinate(18, glm::vec2(1.0f, 1.0f));
	cube.setTextureCoordinate(19, glm::vec2(0.0f, 1.0f));

	// Bottom
	cube.addVertex(glm::vec3(0.5, -0.5, 0.5));
	cube.addVertex(glm::vec3(-0.5, -0.5, 0.5));
	cube.addVertex(glm::vec3(-0.5, -0.5, -0.5));
	cube.addVertex(glm::vec3(0.5, -0.5, -0.5));
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

int main(){
	Graphic graphic;
	graphic.run();
	return 0;
}
