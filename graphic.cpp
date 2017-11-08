#include "graphic.h"

void display(){
	glClear(GL_COLOR_BUFFER_BIT);
	model->loadIdentity();
	model->push();
	model->pop();
	
	glUniformMatrix4fv(glGetUniformLocation(shader_program.getID(), "model"), 1, GL_FALSE, &(*(model->getModel()))[0][0]);
	glBindTexture(GL_TEXTURE_2D, texture->getID());
	glBindVertexArray(VAO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->getNumVertices() * 8, mesh->getVertexData(), GL_DYNAMIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh->getNumTriangles() * 3, mesh->getIndexData(), GL_DYNAMIC_DRAW);
		glDrawElements(GL_TRIANGLES, mesh->getNumTriangles() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glfwSwapBuffers(window);
}

void resizeWindow(GLFWwindow *window, int width, int height){
	glViewport(0, 0, width, height);
}

void checkErrorAt(const char *location){
	GLenum err;
	if((err = glGetError()) != GL_NO_ERROR){
		cout << "Error at " << location << ": " << (err == GL_INVALID_VALUE) << endl;
	}
}

void processInputs(){
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		glfwSetWindowShouldClose(window, true);
	}
}

void mainLoop(){
	while(!glfwWindowShouldClose(window)){
		glfwPollEvents();
		processInputs();
		display();
	}
}

Mesh* createCubeMesh(){
	Mesh *cube = new Mesh();

	cube->setDefaultColor(glm::vec3(1.0f, 0.0f, 1.0f));

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
	// cube->addVertex(glm::vec3(0.5, -0.5, -0.5));
	// cube->addVertex(glm::vec3(-0.5, -0.5, -0.5));
	// cube->addVertex(glm::vec3(-0.5, 0.5, -0.5));
	// cube->addVertex(glm::vec3(0.5, 0.5, -0.5));
	// cube->addTriangle(4, 5, 6);
	// cube->addTriangle(4, 6, 7);
	// cube->setTextureCoordinate(4, glm::vec2(0.0f, 0.0f));
	// cube->setTextureCoordinate(5, glm::vec2(1.0f, 0.0f));
	// cube->setTextureCoordinate(6, glm::vec2(1.0f, 1.0f));
	// cube->setTextureCoordinate(7, glm::vec2(0.0f, 1.0f));

	// // Left
	// cube->addVertex(glm::vec3(-0.5, -0.5, -0.5));
	// cube->addVertex(glm::vec3(-0.5, -0.5, 0.5));
	// cube->addVertex(glm::vec3(-0.5, 0.5, 0.5));
	// cube->addVertex(glm::vec3(-0.5, 0.5, -0.5));
	// cube->addTriangle(8, 9, 10);
	// cube->addTriangle(8, 10, 11);
	// cube->setTextureCoordinate(8, glm::vec2(0.0f, 0.0f));
	// cube->setTextureCoordinate(9, glm::vec2(1.0f, 0.0f));
	// cube->setTextureCoordinate(10, glm::vec2(1.0f, 1.0f));
	// cube->setTextureCoordinate(11, glm::vec2(0.0f, 1.0f));

	// // Right
	// cube->addVertex(glm::vec3(0.5, -0.5, 0.5));
	// cube->addVertex(glm::vec3(0.5, -0.5, -0.5));
	// cube->addVertex(glm::vec3(0.5, 0.5, -0.5));
	// cube->addVertex(glm::vec3(0.5, 0.5, 0.5));
	// cube->addTriangle(12, 14, 14);
	// cube->addTriangle(12, 15, 15);
	// cube->setTextureCoordinate(12, glm::vec2(0.0f, 0.0f));
	// cube->setTextureCoordinate(13, glm::vec2(1.0f, 0.0f));
	// cube->setTextureCoordinate(14, glm::vec2(1.0f, 1.0f));
	// cube->setTextureCoordinate(15, glm::vec2(0.0f, 1.0f));

	// // Top
	// cube->addVertex(glm::vec3(-0.5, 0.5, 0.5));
	// cube->addVertex(glm::vec3(0.5, 0.5, 0.5));
	// cube->addVertex(glm::vec3(0.5, 0.5, -0.5));
	// cube->addVertex(glm::vec3(-0.5, 0.5, -0.5));
	// cube->addTriangle(16, 18, 18);
	// cube->addTriangle(16, 19, 19);
	// cube->setTextureCoordinate(16, glm::vec2(0.0f, 0.0f));
	// cube->setTextureCoordinate(17, glm::vec2(1.0f, 0.0f));
	// cube->setTextureCoordinate(18, glm::vec2(1.0f, 1.0f));
	// cube->setTextureCoordinate(19, glm::vec2(0.0f, 1.0f));

	// // Bottom
	// cube->addVertex(glm::vec3(0.5, -0.5, 0.5));
	// cube->addVertex(glm::vec3(-0.5, -0.5, 0.5));
	// cube->addVertex(glm::vec3(-0.5, -0.5, -0.5));
	// cube->addVertex(glm::vec3(0.5, -0.5, -0.5));
	// cube->addTriangle(20, 22, 22);
	// cube->addTriangle(20, 23, 23);
	// cube->setTextureCoordinate(20, glm::vec2(0.0f, 0.0f));
	// cube->setTextureCoordinate(21, glm::vec2(1.0f, 0.0f));
	// cube->setTextureCoordinate(22, glm::vec2(1.0f, 1.0f));
	// cube->setTextureCoordinate(23, glm::vec2(0.0f, 1.0f));

	return cube;
}

bool init(){
	if(!glfwInit()){
		cout << "Failed to initialize GLFW" << endl;
		return false;
	}

	window = glfwCreateWindow(1024, 1024, "Graphic", NULL, NULL);
	if(window == NULL){
		cout << "Failed to create GLFW window" << endl;
		return false;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, resizeWindow);

	glEnable(GL_PROGRAM_POINT_SIZE);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	Shader vertexShader("vertex_shader.glsl", GL_VERTEX_SHADER);
	if(!vertexShader.compile()){
		return false;
	}
	Shader fragmentShader("fragment_shader.glsl", GL_FRAGMENT_SHADER);
	if(!fragmentShader.compile()){
		return false;
	}

	shader_program = ShaderProgram();
	shader_program.attachShader(vertexShader);
	shader_program.attachShader(fragmentShader);
	if(!shader_program.link()){
		return false;
	}
	shader_program.use();

	mesh = createCubeMesh();
	mesh->setColor(2, glm::vec3(1.0f, 1.0f, 0.0f));

	texture = new Texture("./texture.png");

	model = new ModelViewMatrix();
	
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
	glUniform1i(glGetUniformLocation(shader_program.getID(), "texture1"), 0);
	glUniformMatrix4fv(glGetUniformLocation(shader_program.getID(), "model"), 1, GL_FALSE, &(*(model->getModel()))[0][0]);
	
	checkErrorAt("init");
	return true;
}

int main(){
	if(!init()){
		return -1;
	}
	mainLoop();

	delete texture;
	delete model;
	delete mesh;
	glfwTerminate();
	return 0;
}
