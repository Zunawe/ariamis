#ifndef OBJECT_H
#define OBJECT_H

#include <functional>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "renderer.h"

class Object{
	public:
		Object();
		void update();
		void addBehavior(std::string name, std::function<void(Object*)> func);
		void removeBehavior(std::string name);
        void translate(glm::vec3 v);
        void scale(glm::vec3 v);
        void scale(float a);
        void rotate(float theta, glm::vec3 axis);
		glm::mat4 getModel();

		glm::vec3 position;
		glm::vec3 scales;
		glm::quat rotation;
		Renderer renderer;

	private:
		std::map<std::string, std::function<void(Object*)>> behaviors;
};

#endif
