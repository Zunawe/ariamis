#include "object.h"

Object::Object(){
	scales = glm::vec3(1.0f, 1.0f, 1.0f);
	update = [](std::shared_ptr<Object> me){
		// Empty
	};
}

void Object::translate(glm::vec3 delta){
	position += delta;
}

void Object::scale(glm::vec3 delta){
	scales.x *= delta.x;
	scales.y *= delta.y;
	scales.z *= delta.z;
}

void Object::rotate(float angle, glm::vec3 delta){
	// Not Implemented
}

glm::mat4 Object::getModel(){
	glm::mat4 model = glm::mat4(1);
	model = glm::translate(model, position);
	// Rotate
	model = glm::scale(model, scales);
	return model;
}
