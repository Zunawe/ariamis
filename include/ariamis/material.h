#ifndef MATERIAL_H
#define MATERIAL_H

#include <memory>

#include <glm/glm.hpp>

#include <ariamis/texture.h>

namespace Ariamis {
	class Material{
		public:
			Material();
			void setDiffuseMap(std::shared_ptr<Texture> texture);
			void setSpecularMap(std::shared_ptr<Texture> texture);

			glm::vec3 ambient;
			glm::vec3 diffuse;
			std::shared_ptr<Texture> diffuseMap;
			glm::vec3 specular;
			std::shared_ptr<Texture> specularMap;
			float shininess;
	};
	bool operator==(const Material &lhs, const Material &rhs);
}

#endif
