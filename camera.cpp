#include "camera.hpp"

Camera::Camera(){
	this->position = glm::vec3(0, 0, 0);
	this->up = glm::vec3(0, 1, 0);
	this->forward = glm::vec3(0, 0, 1);
}

glm::vec3 Camera::getPosition(){
	return this->position;
}

void Camera::setPosition(glm::vec3 newPosition){
	this->position = newPosition;
}

glm::vec3 Camera::getUp(){
	return this->up;
}

void Camera::setUp(glm::vec3 newUp){
	this->up = newUp;
}

glm::vec3 Camera::getForward(){
	return this->forward;
}

void Camera::setForward(glm::vec3 newForward){
	this->forward = newForward;
}