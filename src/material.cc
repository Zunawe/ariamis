#include <ariamis/material.h>

namespace Ariamis {
	Material Material::DEFAULT_MATERIAL;

	/**
	* Sets the default material properties.
	* Diffuse and specular maps are set to the default texture on their construction.
	*/
	Material::Material(){
		ambient = glm::vec3(1.0f, 1.0f, 1.0f);
		diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		specular = glm::vec3(1.0f, 1.0f, 1.0f);
		shininess = 32.0f;

		diffuseMap = &Texture::DEFAULT_TEXTURE;
		specularMap = &Texture::DEFAULT_TEXTURE;
	}

	bool operator==(const Material &lhs, const Material &rhs){
		return lhs.ambient == rhs.ambient &&
					lhs.diffuse == rhs.diffuse &&
					lhs.diffuseMap->getID() == rhs.diffuseMap->getID() &&
				lhs.specular == rhs.specular &&
					lhs.specularMap->getID() == rhs.specularMap->getID() &&
				lhs.shininess == rhs.shininess;
	}
}
