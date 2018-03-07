#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "object.h"
#include "object_renderer.h"

class Scene{
	public:
		Scene();
		void draw();

		std::vector<std::shared_ptr<Object>> objects;
		std::vector<std::shared_ptr<Light>> lights;
		std::vector<Camera> cameras;

	private:
		glm::mat4 projection;
};

#endif
