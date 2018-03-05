#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

#include "shader.h"

class Light{
	public:
		glm::vec4 position;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		virtual bool setUniforms(Shader shader, std::string lightLocation) const;
};

class PointLight: public Light{
	public:
		float kc;
		float kl;
		float kq;

		bool setUniforms(Shader shader, std::string lightLocation) const;
};

class SpotLight: public Light{
	public:
		glm::vec3 direction;
		float cosAngle;

		bool setUniforms(Shader shader, std::string lightLocation) const;
};

#endif
