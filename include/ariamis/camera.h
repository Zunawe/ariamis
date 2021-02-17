#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

namespace Ariamis {
	class Camera{
		public:
			Camera();

			glm::vec3 getPosition() const;
			void setPosition(glm::vec3 newPosition);
			glm::vec3 getForward();
			void setForward(glm::vec3 newForward);
			glm::vec3 getUp();
			void setUp(glm::vec3 newUp);
			glm::mat4 getViewMatrix();
			glm::vec3 getRight();

			void translate(glm::vec3 vector);
			void translateRelative(glm::vec3 vector);
			void move(glm::vec3 vector, float speed);
			void moveRelative(glm::vec3 vector, float speed);
			void lookAt(glm::vec3 target);
			void lookAt(glm::vec3 target, glm::vec3 up);

		private:
			void recalculateRight();

			glm::vec3 position;
			glm::vec3 forward;
			glm::vec3 up;
			glm::vec3 right;
	};
}

#endif
