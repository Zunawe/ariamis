#include "material.hpp"

Material Material::DEFAULT_MATERIAL;

Material::Material(){
	ambient = glm::vec3(0.7f, 0.7f, 0.7f);
	diffuse = glm::vec3(0.7f, 0.7f, 0.7f);
	specular = glm::vec3(0.4f, 0.4f, 0.4f);
	shininess = 32.0f;
}

bool operator==(const Material &lhs, const Material &rhs){
	return lhs.ambient == rhs.ambient &&
	       lhs.ambientMap.getID() == rhs.ambientMap.getID() &&
	       lhs.diffuse == rhs.diffuse &&
	       lhs.diffuseMap.getID() == rhs.diffuseMap.getID() &&
		   lhs.specular == rhs.specular &&
	       lhs.specularMap.getID() == rhs.specularMap.getID() &&
		   lhs.shininess == rhs.shininess;
}
