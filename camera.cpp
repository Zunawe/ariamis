#include "camera.hpp"

Camera::Camera(){
	this->position = glm::vec3(0, 0, 0);
	this->up = glm::vec3(0, 1, 0);
	this->forward = glm::vec3(0, 0, 1);
	recalculateRight();
}

glm::vec3 Camera::getPosition() const{
	return this->position;
}

void Camera::setPosition(glm::vec3 newPosition){
	this->position = newPosition;
}

void Camera::translate(glm::vec3 vector){
	this->position += vector;
}

void Camera::translateRelative(glm::vec3 vector){
	this->position += (vector.x * this->right) + (vector.y * this->up) + (vector.z * this->forward);
}

void Camera::move(glm::vec3 vector, float velocity){
	translate(velocity * glm::normalize(vector));
}

void Camera::moveRelative(glm::vec3 vector, float velocity){
	translateRelative(velocity * glm::normalize(vector));
}

glm::vec3 Camera::getUp(){
	return this->up;
}

void Camera::setUp(glm::vec3 newUp){
	this->up = glm::normalize(newUp);
	recalculateRight();
}

glm::vec3 Camera::getForward(){
	return this->forward;
}

void Camera::setForward(glm::vec3 newForward){
	this->forward = glm::normalize(newForward);
	recalculateRight();
}

void Camera::lookAt(glm::vec3 target){
	this->forward = glm::normalize(target - this->position);
	recalculateRight();
}

void Camera::lookAt(glm::vec3 target, glm::vec3 up){
	this->forward = glm::normalize(target - this->position);
	this->up = glm::normalize(up);
	recalculateRight();
}

glm::mat4 Camera::getViewMatrix(){
	return glm::lookAt(this->position, this->position + this->forward, this->up);
}

void Camera::recalculateRight(){
	right = glm::normalize(glm::cross(up, -forward));
}

glm::vec3 Camera::getRight(){
	return this->right;
}