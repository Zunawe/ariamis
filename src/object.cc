#include "object.h"

#include <glm/gtc/matrix_transform.hpp>

Object::Object(){
    position = glm::vec3(0);
    scales = glm::vec3(1);
    rotation = glm::quat(1, 0, 0, 0);

	update = [](std::shared_ptr<Object> /*me*/){
		// Empty
	};
}

void Object::translate(glm::vec3 v){
    position += v;
}

void Object::scale(glm::vec3 v){
    scales *= v;
}

void Object::scale(float a){
    scale(glm::vec3(a));
}

void Object::rotate(float theta, glm::vec3 axis){
    rotation = glm::rotate(rotation, theta, axis);
}

/**
 * Gets a model matrix according to the object's current position, scale, and
 * rotation vectors. Objects are always transformed in this order: scale, rotate,
 * translate.
 * 
 * @return the model matrix corresponding to this object's transformation vectors.
 */
glm::mat4 Object::getModel(){
    glm::mat4 model = glm::mat4(1);
	model = glm::translate(model, position);
	model = glm::scale(model, scales);
	model = ((glm::mat4)rotation) * model;
    return model;
}
