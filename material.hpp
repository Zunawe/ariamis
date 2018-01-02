#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>

#include "texture.hpp"

class Material{
	public:
		static Material DEFAULT_MATERIAL;

		Material();

		glm::vec3 ambient;
		glm::vec3 diffuse;
		Texture diffuseMap;
		glm::vec3 specular;
		Texture specularMap;
		float shininess;
};
bool operator==(const Material &lhs, const Material &rhs);

#endif
