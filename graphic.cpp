#include "graphic.h"

void display(){
	glClear(GL_COLOR_BUFFER_BIT);

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

Mesh* getRectangleMesh(float width, float height){
	Mesh *rectMesh = new Mesh();

	rectMesh->addVertex(glm::vec2(-width / 2, -height / 2));
	rectMesh->addVertex(glm::vec2(width / 2, -height / 2));
	rectMesh->addVertex(glm::vec2(width / 2, height / 2));
	rectMesh->addVertex(glm::vec2(-width / 2, height / 2));

	rectMesh->addTriangle(0, 1, 2);
	rectMesh->addTriangle(0, 2, 3);
	rectMesh->setTextureCoordinate(0, glm::vec2(0.0f, 0.0f));
	rectMesh->setTextureCoordinate(1, glm::vec2(1.0f, 0.0f));
	rectMesh->setTextureCoordinate(2, glm::vec2(1.0f, 1.0f));
	rectMesh->setTextureCoordinate(3, glm::vec2(0.0f, 1.0f));

	return rectMesh;
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

	mesh = getRectangleMesh(1.0f, 1.0f);
	mesh->setColor(2, glm::vec3(1.0f, 0.0f, 0.0f));

	texture = new Texture("./texture.png");
	
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

	checkErrorAt("init");
	return true;
}

int main(){
	if(!init()){
		return -1;
	}
	mainLoop();

	glfwTerminate();
	return 0;
}
