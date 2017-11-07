#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

class ModelViewMatrix{
	public:
		ModelViewMatrix();

		void loadIdentity();

		void translate(glm::vec2 vector);
		void translate(glm::vec3 vector);
		
		void scale(float scalar);
		void scale(glm::vec2 vector);
		void scale(glm::vec3 vector);

		void rotate(float theta, glm::vec3 axis);

		void push();
		void pop();

		glm::mat4* getModel();

		void prettyPrint();
	
	private:
		glm::mat4 *model;
		std::vector<glm::mat4 *> modelStack;
};