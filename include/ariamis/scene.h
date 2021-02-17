#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <vector>

#include <ariamis/object.h>

namespace Ariamis {
	class Scene{
		public:
			Scene();
			void update();
			void draw();

			std::vector<std::shared_ptr<Object>> objects;
			std::vector<std::shared_ptr<Light>> lights;
			Camera camera;
	};
}

#endif
