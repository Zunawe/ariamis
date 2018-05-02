#include "object.h"

Object::Object(){
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	update = [](std::shared_ptr<Object> /*me*/){
		// Empty
	};
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
	// Rotate
	model = glm::scale(model, scale);
	return model;
}
