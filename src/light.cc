#include "light.h"

/**
 * Sets the values for a light struct in the provided shader.
 * 
 * @param shader the shader to set the uniforms on.
 * @param lightLocation the location of the light struct in the shader
 * returned by OpenGL
 */
bool Light::setUniforms(Shader shader, std::string lightLocation) const{
	bool allValid = true;
	allValid = allValid && shader.setUniform(lightLocation + ".position", position);
	allValid = allValid && shader.setUniform(lightLocation + ".ambient", ambient);
	allValid = allValid && shader.setUniform(lightLocation + ".diffuse", diffuse);
	allValid = allValid && shader.setUniform(lightLocation + ".specular", specular);
	return allValid;
}

/**
 * Sets the values for a light struct in the provided shader.
 * 
 * @param shader the shader to set the uniforms on.
 * @param lightLocation the location of the light struct in the shader
 * returned by OpenGL
 */
bool PointLight::setUniforms(Shader shader, std::string lightLocation) const{
	bool allValid = true;
	allValid = allValid && Light::setUniforms(shader, lightLocation);
	allValid = allValid && shader.setUniform(lightLocation + ".kc", kc);
	allValid = allValid && shader.setUniform(lightLocation + ".kl", kl);
	allValid = allValid && shader.setUniform(lightLocation + ".kq", kq);
	return allValid;
}

/**
 * Sets the values for a light struct in the provided shader.
 * 
 * @param shader the shader to set the uniforms on.
 * @param lightLocation the location of the light struct in the shader
 * returned by OpenGL
 */
bool SpotLight::setUniforms(Shader shader, std::string lightLocation) const{
	bool allValid = true;
	allValid = allValid && Light::setUniforms(shader, lightLocation);
	allValid = allValid && shader.setUniform(lightLocation + ".direction", direction);
	allValid = allValid && shader.setUniform(lightLocation + ".cosAngle", cosAngle);
	return allValid;
}
