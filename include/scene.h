#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "object.h"

class Scene{
	public:
		Scene();
		void update();
		void draw();

		std::vector<std::shared_ptr<Object>> objects;
		std::vector<std::shared_ptr<Light>> lights;
		std::vector<Camera> cameras;
		glm::mat4 projection;
};

#endif
