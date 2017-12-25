#include "graphic.hpp"

using namespace std;

int main(){
	if(!glfwInit()){
		cout << "Failed to initialize GLFW" << endl;
	}

	GLFWwindow *window = glfwCreateWindow(1024, 1024, "Graphic", NULL, NULL);
	if(window == NULL){
		cout << "Failed to create GLFW window" << endl;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);


	ContextManager contextManager(window);
	contextManager.run();
}

void initGraphics(){
}
