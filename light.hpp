#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

class Light{
	public:
		glm::vec3 position;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
};

class PointLight: public Light{
	public:
		float kc;
		float kl;
		float kq;
};

class SpotLight: public Light{
	public:
		glm::vec3 direction;
		float angle;
};

#endif
