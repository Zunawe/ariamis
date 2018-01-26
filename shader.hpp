#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <exception>

#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Shader{
	public:
		static Shader DEFAULT_SHADER;

		void loadSources(const GLchar *vertexShaderPath, const GLchar *fragmentShaderPath);
		void use();
		unsigned int getID();

		bool setUniform(std::string name, const int &value);
		bool setUniform(std::string name, const float &value);
		bool setUniform(std::string name, const glm::vec2 &value);
		bool setUniform(std::string name, const glm::vec3 &value);
		bool setUniform(std::string name, const glm::vec4 &value);
		bool setUniform(std::string name, const glm::mat2 &value);
		bool setUniform(std::string name, const glm::mat3 &value);
		bool setUniform(std::string name, const glm::mat4 &value);
	
	private:
		unsigned int id;
		void checkCompilation(unsigned int shaderID);
		void checkLinking();
};

struct ShaderException : public std::exception{
	std::string shaderLog;

	const char* what() const throw(){
		return shaderLog.c_str();
	}

	public:
		ShaderException(const char *infoLog){
			shaderLog = infoLog;
		}
};

#endif
