#include "model_view_matrix.h"

ModelViewMatrix::ModelViewMatrix(){
	model = new glm::mat4();
}

void ModelViewMatrix::loadIdentity(){
	modelStack.clear();
	*model = glm::mat4();
}

void ModelViewMatrix::translate(glm::vec2 vector){
	translate(glm::vec3(vector, 0.0f));
}

void ModelViewMatrix::translate(glm::vec3 vector){
	*model = glm::translate(*model, vector);
}

void ModelViewMatrix::scale(float scalar){
	scale(glm::vec3(scalar));
}

void ModelViewMatrix::scale(glm::vec2 vector){
	scale(glm::vec3(vector, 1.0f));
}

void ModelViewMatrix::scale(glm::vec3 vector){
	*model = glm::scale(*model, vector);
}

void ModelViewMatrix::rotate(float theta, glm::vec3 axis){
	*model = glm::rotate(*model, glm::radians(theta), axis);
}

glm::mat4* ModelViewMatrix::getModel(){
	return model;
}

void ModelViewMatrix::push(){
	modelStack.push_back(model);
	model = new glm::mat4();
}

void ModelViewMatrix::pop(){
	model = modelStack[modelStack.size() - 1];
	modelStack.pop_back();
}

void ModelViewMatrix::prettyPrint(){
	printf("[%f, %f, %f\n %f, %f, %f\n %f, %f, %f]\n", (*model)[0][0], (*model)[0][1], (*model)[0][2], (*model)[1][0], (*model)[1][1], (*model)[1][2], (*model)[2][0], (*model)[2][1], (*model)[2][2]);	
}