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

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	mesh = createCubeMesh();
	texture = new Texture("./texture.png");
	
	shaderProgram.init("vertex_shader.glsl", "fragment_shader.glsl");
	shaderProgram.use();
	glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
	glBindVertexArray(0);
	glUniform1i(glGetUniformLocation(shaderProgram.getID(), "texture1"), 0);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.getID(), "model"), 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.getID(), "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.getID(), "projection"), 1, GL_FALSE, &projection[0][0]);

	theta = 0.0f;
	phi = 90.0f;
	
	checkErrorAt("Graphic Constructor");
}

void Graphic::display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	model = glm::mat4(1);

	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0, 0, 1));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0, 1, 0));

	view = glm::mat4(1);
	view = glm::translate(view, glm::vec3(0, 0, -3));
	view = glm::rotate(view, glm::radians((float)phi + 90), glm::vec3(1, 0, 0));
	view = glm::rotate(view, glm::radians((float)theta), glm::vec3(0, 1, 0));

	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
	
	shaderProgram.use();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.getID(), "model"), 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.getID(), "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.getID(), "projection"), 1, GL_FALSE, &projection[0][0]);
	glBindTexture(GL_TEXTURE_2D, texture->getID());
	glBindVertexArray(VAO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->getNumVertices() * 8, mesh->getVertexData(), GL_DYNAMIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh->getNumTriangles() * 3, mesh->getIndexData(), GL_DYNAMIC_DRAW);
		glDrawElements(GL_TRIANGLES, mesh->getNumTriangles() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

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

Mesh* Graphic::createCubeMesh(){
	Mesh *cube = new Mesh();

	// cube->setDefaultColor(glm::vec3(1.0f, 0.0f, 1.0f));

	// Front
	cube->addVertex(glm::vec3(-0.5, -0.5, 0.5));
	cube->addVertex(glm::vec3(0.5, -0.5, 0.5));
	cube->addVertex(glm::vec3(0.5, 0.5, 0.5));
	cube->addVertex(glm::vec3(-0.5, 0.5, 0.5));
	cube->addTriangle(0, 1, 2);
	cube->addTriangle(0, 2, 3);
	cube->setTextureCoordinate(0, glm::vec2(0.0f, 0.0f));
	cube->setTextureCoordinate(1, glm::vec2(1.0f, 0.0f));
	cube->setTextureCoordinate(2, glm::vec2(1.0f, 1.0f));
	cube->setTextureCoordinate(3, glm::vec2(0.0f, 1.0f));
	
	// Back
	cube->addVertex(glm::vec3(0.5, -0.5, -0.5));
	cube->addVertex(glm::vec3(-0.5, -0.5, -0.5));
	cube->addVertex(glm::vec3(-0.5, 0.5, -0.5));
	cube->addVertex(glm::vec3(0.5, 0.5, -0.5));
	cube->addTriangle(4, 5, 6);
	cube->addTriangle(4, 6, 7);
	cube->setTextureCoordinate(4, glm::vec2(0.0f, 0.0f));
	cube->setTextureCoordinate(5, glm::vec2(1.0f, 0.0f));
	cube->setTextureCoordinate(6, glm::vec2(1.0f, 1.0f));
	cube->setTextureCoordinate(7, glm::vec2(0.0f, 1.0f));

	// Left
	cube->addVertex(glm::vec3(-0.5, -0.5, -0.5));
	cube->addVertex(glm::vec3(-0.5, -0.5, 0.5));
	cube->addVertex(glm::vec3(-0.5, 0.5, 0.5));
	cube->addVertex(glm::vec3(-0.5, 0.5, -0.5));
	cube->addTriangle(8, 9, 10);
	cube->addTriangle(8, 10, 11);
	cube->setTextureCoordinate(8, glm::vec2(0.0f, 0.0f));
	cube->setTextureCoordinate(9, glm::vec2(1.0f, 0.0f));
	cube->setTextureCoordinate(10, glm::vec2(1.0f, 1.0f));
	cube->setTextureCoordinate(11, glm::vec2(0.0f, 1.0f));

	// Right
	cube->addVertex(glm::vec3(0.5, -0.5, 0.5));
	cube->addVertex(glm::vec3(0.5, -0.5, -0.5));
	cube->addVertex(glm::vec3(0.5, 0.5, -0.5));
	cube->addVertex(glm::vec3(0.5, 0.5, 0.5));
	cube->addTriangle(12, 13, 14);
	cube->addTriangle(12, 14, 15);
	cube->setTextureCoordinate(12, glm::vec2(0.0f, 0.0f));
	cube->setTextureCoordinate(13, glm::vec2(1.0f, 0.0f));
	cube->setTextureCoordinate(14, glm::vec2(1.0f, 1.0f));
	cube->setTextureCoordinate(15, glm::vec2(0.0f, 1.0f));

	// Top
	cube->addVertex(glm::vec3(-0.5, 0.5, 0.5));
	cube->addVertex(glm::vec3(0.5, 0.5, 0.5));
	cube->addVertex(glm::vec3(0.5, 0.5, -0.5));
	cube->addVertex(glm::vec3(-0.5, 0.5, -0.5));
	cube->addTriangle(16, 17, 18);
	cube->addTriangle(16, 18, 19);
	cube->setTextureCoordinate(16, glm::vec2(0.0f, 0.0f));
	cube->setTextureCoordinate(17, glm::vec2(1.0f, 0.0f));
	cube->setTextureCoordinate(18, glm::vec2(1.0f, 1.0f));
	cube->setTextureCoordinate(19, glm::vec2(0.0f, 1.0f));

	// Bottom
	cube->addVertex(glm::vec3(0.5, -0.5, 0.5));
	cube->addVertex(glm::vec3(-0.5, -0.5, 0.5));
	cube->addVertex(glm::vec3(-0.5, -0.5, -0.5));
	cube->addVertex(glm::vec3(0.5, -0.5, -0.5));
	cube->addTriangle(20, 21, 22);
	cube->addTriangle(20, 22, 23);
	cube->setTextureCoordinate(20, glm::vec2(0.0f, 0.0f));
	cube->setTextureCoordinate(21, glm::vec2(1.0f, 0.0f));
	cube->setTextureCoordinate(22, glm::vec2(1.0f, 1.0f));
	cube->setTextureCoordinate(23, glm::vec2(0.0f, 1.0f));

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
