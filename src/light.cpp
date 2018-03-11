#include "light.h"

/**
 * Sets the values for a light struct in the provided shader.
 * 
 * @param shader the shader to set the uniforms on.
 * @param lightLocation the location of the light struct in the shader
 * returned by OpenGL
 */
bool Light::setUniforms(Shader shader, std::string lightLocation) const{
	shader.setUniform(lightLocation + ".position", position);
	shader.setUniform(lightLocation + ".ambient", ambient);
	shader.setUniform(lightLocation + ".diffuse", diffuse);
	shader.setUniform(lightLocation + ".specular", specular);
}

/**
 * Sets the values for a light struct in the provided shader.
 * 
 * @param shader the shader to set the uniforms on.
 * @param lightLocation the location of the light struct in the shader
 * returned by OpenGL
 */
bool PointLight::setUniforms(Shader shader, std::string lightLocation) const{
	Light::setUniforms(shader, lightLocation);
	shader.setUniform(lightLocation + ".kc", kc);
	shader.setUniform(lightLocation + ".kl", kl);
	shader.setUniform(lightLocation + ".kq", kq);
}

/**
 * Sets the values for a light struct in the provided shader.
 * 
 * @param shader the shader to set the uniforms on.
 * @param lightLocation the location of the light struct in the shader
 * returned by OpenGL
 */
bool SpotLight::setUniforms(Shader shader, std::string lightLocation) const{
	Light::setUniforms(shader, lightLocation);
	shader.setUniform(lightLocation + ".direction", direction);
	shader.setUniform(lightLocation + ".cosAngle", cosAngle);
}
