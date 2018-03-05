#include "light.h"

bool Light::setUniforms(Shader shader, std::string lightLocation) const{
	shader.setUniform(lightLocation + ".position", position);
	shader.setUniform(lightLocation + ".ambient", ambient);
	shader.setUniform(lightLocation + ".diffuse", diffuse);
	shader.setUniform(lightLocation + ".specular", specular);
}

bool PointLight::setUniforms(Shader shader, std::string lightLocation) const{
	Light::setUniforms(shader, lightLocation);
	shader.setUniform(lightLocation + ".kc", kc);
	shader.setUniform(lightLocation + ".kl", kl);
	shader.setUniform(lightLocation + ".kq", kq);
}

bool SpotLight::setUniforms(Shader shader, std::string lightLocation) const{
	Light::setUniforms(shader, lightLocation);
	shader.setUniform(lightLocation + ".direction", direction);
	shader.setUniform(lightLocation + ".cosAngle", cosAngle);
}
