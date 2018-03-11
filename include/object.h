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
		glm::mat4 getModel();

		ObjectRenderer renderer;
		void (*update)(std::shared_ptr<Object>);
		glm::vec3 position;
		glm::vec3 scale;
		glm::vec4 rotation;
};

#endif
