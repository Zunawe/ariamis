#include "graphic.h"

void initVertices(){
	vertices.push_back(glm::vec3(-0.5f, -0.5f, 0.0f));
	vertices.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

	vertices.push_back(glm::vec3(0.5f, -0.5f, 0.0f));
	vertices.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

	vertices.push_back(glm::vec3(0.0f, 0.5f, 0.0f));
	vertices.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT);

	vector<unsigned int> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	shader_program.use();
	glBindVertexArray(VAO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices[0].x, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

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

bool init(){
	if(!glfwInit()){
		cout << "Failed to initialize GLFW" << endl;
		return false;
	}

	window = glfwCreateWindow(1024, 1024, "Meaningless UI Component", NULL, NULL);
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

	initVertices();
	glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
	glBindVertexArray(0);

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
