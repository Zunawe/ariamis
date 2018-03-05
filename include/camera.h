#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera{
	public:
		Camera();
		glm::vec3 getPosition() const;
		void setPosition(glm::vec3 newPosition);
		void translate(glm::vec3 vector);
		void translateRelative(glm::vec3 vector);
		void move(glm::vec3 vector, float speed);
		void moveRelative(glm::vec3 vector, float speed);
		glm::vec3 getForward();
		void setForward(glm::vec3 newForward);
		glm::vec3 getUp();
		void setUp(glm::vec3 newUp);
		void lookAt(glm::vec3 target);
		void lookAt(glm::vec3 target, glm::vec3 up);
		glm::mat4 getViewMatrix();
		glm::vec3 getRight();

	private:
		glm::vec3 position;
		glm::vec3 forward;
		glm::vec3 up;
		glm::vec3 right;

		void recalculateRight();
};
#endif