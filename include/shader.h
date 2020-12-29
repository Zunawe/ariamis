#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <exception>
#include <string>
#include <vector>

#include "glad/glad.h"
#include <glm/glm.hpp>

class Shader{
	public:
		void loadFile(const GLchar *sourcePath, GLenum type);
		void loadSource(const char *source, GLenum type);
		void link();
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
		void checkCompilation(unsigned int shaderID);
		void checkLinking();
		
		unsigned int id;
		std::vector<unsigned int> shaders;
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
