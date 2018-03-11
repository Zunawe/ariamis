#ifndef OBJECT_H
#define OBJECT_H

#include <functional>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "object_renderer.h"

class Object{
	public:
		Object();
		void translate(glm::vec3 delta);
		void scale(glm::vec3 delta);
		void rotate(float angle, glm::vec3 delta);
		glm::mat4 getModel();

		ObjectRenderer renderer;
		void (*update)(std::shared_ptr<Object>);
		glm::vec3 position;
		glm::vec3 scales;
		glm::vec4 rotation;
};

#endif
