#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera{
	public:
		Camera();
		glm::vec3 getPosition();
		void setPosition(glm::vec3 newPosition);
		glm::vec3 getUp();
		void setUp(glm::vec3 newUp);
		glm::vec3 getForward();
		void setForward(glm::vec3 newForward);

	private:
		glm::vec3 position;
		glm::vec3 up;
		glm::vec3 forward;
};
#endif