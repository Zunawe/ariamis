#include "camera.h"

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

/**
 * Translates the camera by world coordinates.
 * 
 * @param vector the translation vector.
 */
void Camera::translate(glm::vec3 vector){
	this->position += vector;
}

/**
 * Translates the camera assuming forward is +z and up is +y.
 * @param vector the translation vector.
 */
void Camera::translateRelative(glm::vec3 vector){
	this->position += (vector.x * this->right) + (vector.y * this->up) + (vector.z * this->forward);
}

/**
 * A macro for translation by some distance in world coordinates.
 * Calls translate.
 * 
 * @param vector the direction vector.
 * @param speed the distance to translate by.
 */
void Camera::move(glm::vec3 vector, float speed){
	translate(speed * glm::normalize(vector));
}

/**
 * A macro for translation by some distance in a coordinate system where forward is +z and up is +y.
 * Calls translateRelative.
 * 
 * @param vector the direction vector.
 * @param speed the distance to translate by.
 */
void Camera::moveRelative(glm::vec3 vector, float speed){
	translateRelative(speed * glm::normalize(vector));
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

/**
 * Without changing the up vector, point the camera at the target.
 * 
 * @param target the position to point the camera toward.
 */
void Camera::lookAt(glm::vec3 target){
	this->forward = glm::normalize(target - this->position);
	recalculateRight();
}

/**
 * Point the camera at the target.
 * 
 * @param target the position to point the camera toward.
 * @param up the new up vector.
 */
void Camera::lookAt(glm::vec3 target, glm::vec3 up){
	this->forward = glm::normalize(target - this->position);
	this->up = glm::normalize(up);
	recalculateRight();
}

/**
 * Use the properties of the camera to generate a view matrix.
 * 
 * @return a 4x4 matrix representing a the view.
 */
glm::mat4 Camera::getViewMatrix(){
	return glm::lookAt(this->position, this->position + this->forward, this->up);
}

/**
 * Recalculates the right-pointing vector based on the current up and forward vectors.
 */
void Camera::recalculateRight(){
	right = glm::normalize(glm::cross(up, -forward));
}

glm::vec3 Camera::getRight(){
	return this->right;
}