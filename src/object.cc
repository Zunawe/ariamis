#include <ariamis/object.h>

#include <glm/gtc/matrix_transform.hpp>

Object::Object(){
    position = glm::vec3(0);
    scales = glm::vec3(1);
    rotation = glm::quat(1, 0, 0, 0);
}

void Object::update(){
    for(auto it = behaviors.begin(); it != behaviors.end(); ++it){
		(it->second)(this);
	}
}

void Object::addBehavior(std::string name, std::function<void(Object*)> func){
    behaviors[name] = func;
}

void Object::removeBehavior(std::string name){
    behaviors.erase(name);
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
    glm::mat4 model(1);
	model = glm::translate(model, position);
	model = glm::scale(model, scales);
	model = model * glm::mat4_cast(rotation);
    return model;
}
