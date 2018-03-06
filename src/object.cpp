#include "object.h"

Object::Object(){
	model = glm::mat4(1);
}

glm::mat4 Object::getModel(){
	return model;
}
