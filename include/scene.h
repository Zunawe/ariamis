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
		Scene(int width, int height);
		void addObject(std::shared_ptr<Object> o);
		void draw();

		std::vector<std::shared_ptr<Object>> objects;
		std::vector<std::shared_ptr<Light>> lights;

	private:
		std::vector<Camera> cameras;
		glm::mat4 projection;
};

#endif
