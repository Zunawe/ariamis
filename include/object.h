#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>

#include "object_renderer.h"

class Object{
	public:
		Object();
		glm::mat4 getModel();

		ObjectRenderer renderer;
		glm::mat4 model;

	private:
};

#endif
