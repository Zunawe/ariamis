#ifndef OBJECT_H
#define OBJECT_H

#include <functional>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "renderer.h"

class Object{
	public:
		Object();
        void translate(glm::vec3 v);
        void scale(glm::vec3 v);
        void scale(float a);
        void rotate(float theta, glm::vec3 axis);
		glm::mat4 getModel();

		glm::vec3 position;
		glm::vec3 scales;
		glm::quat rotation;
		Renderer renderer;
		void (*update)(std::shared_ptr<Object>);
};

#endif
