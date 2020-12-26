#include "shader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>

Shader Shader::DEFAULT_SHADER;

/**
 * Loads the source code for a shader
 * 
 * @param sourcePath the path to the shader.
 */
void Shader::loadFile(const char *sourcePath, GLenum type){
	// Read source code
	std::ifstream fileStream;
	fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	std::string source;

	try{
		fileStream.open(sourcePath);

		std::stringstream ss;
		ss << fileStream.rdbuf();

		fileStream.close();
		source = ss.str();
	}
	catch(std::ifstream::failure &e){
		char message[23 + strlen(sourcePath)];
		strcpy(message, "Failed to open file at ");
		strcat(message, sourcePath);
		throw std::runtime_error(message);
	}

	loadSource(source.c_str(), type);
}

void Shader::loadSource(const char *source, GLenum type){
	unsigned int shaderID = glCreateShader(type);
	glShaderSource(shaderID, 1, &source, NULL);
	glCompileShader(shaderID);
	checkCompilation(shaderID);
	shaders.push_back(shaderID);
}

void Shader::link(){
	this->id = glCreateProgram();
	for(auto it = shaders.begin(); it != shaders.end(); ++it){
		glAttachShader(this->id, *it);
	}
	glLinkProgram(this->id);
	checkLinking();

	for(auto it = shaders.begin(); it != shaders.end(); ++it){
		glDeleteShader(*it);
	}
}

/**
 * Checks whether a shader compiled correctly.
 * 
 * @param shaderID the OpenGL id of the shader in question.
 */
void Shader::checkCompilation(unsigned int shaderID){
	int success;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

	if(!success){
		int logSize;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logSize);
		char infoLog[logSize];
		glGetShaderInfoLog(shaderID, logSize, &logSize, &infoLog[0]);

		throw ShaderException(infoLog);
	}
}

/**
 * Checks whether this shader program linked correctly.
 */
void Shader::checkLinking(){
	int success;
	glGetProgramiv(this->id, GL_LINK_STATUS, &success);

	if(!success){
		int logSize;
		glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &logSize);
		char infoLog[logSize];
		glGetProgramInfoLog(this->id, logSize, &logSize, &infoLog[0]);

		throw ShaderException(infoLog);
	}
}

bool Shader::setUniform(std::string name, const int &value){
	use();
	int location = glGetUniformLocation(this->id, name.c_str());
	if(location < 0){
		return false;
	}
	glUniform1i(location, value);
	return true;
}

bool Shader::setUniform(std::string name, const float &value){
	use();
	int location = glGetUniformLocation(this->id, name.c_str());
	if(location < 0){
		return false;
	}
	glUniform1f(location, value);
	return true;
}

bool Shader::setUniform(std::string name, const glm::vec2 &value){
	use();
	int location = glGetUniformLocation(this->id, name.c_str());
	if(location < 0){
		return false;
	}
	glUniform2fv(location, 1, &value[0]);
	return true;
}

bool Shader::setUniform(std::string name, const glm::vec3 &value){
	use();
	int location = glGetUniformLocation(this->id, name.c_str());
	if(location < 0){
		return false;
	}
	glUniform3fv(location, 1, &value[0]);
	return true;
}

bool Shader::setUniform(std::string name, const glm::vec4 &value){
	use();
	int location = glGetUniformLocation(this->id, name.c_str());
	if(location < 0){
		return false;
	}
	glUniform4fv(location, 1, &value[0]);
	return true;
}

bool Shader::setUniform(std::string name, const glm::mat2 &value){
	use();
	int location = glGetUniformLocation(this->id, name.c_str());
	if(location < 0){
		return false;
	}
	glUniformMatrix2fv(location, 1, GL_FALSE, &value[0][0]);
	return true;
}

bool Shader::setUniform(std::string name, const glm::mat3 &value){
	use();
	int location = glGetUniformLocation(this->id, name.c_str());
	if(location < 0){
		return false;
	}
	glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
	return true;
}

bool Shader::setUniform(std::string name, const glm::mat4 &value){
	use();
	int location = glGetUniformLocation(this->id, name.c_str());
	if(location < 0){
		return false;
	}
	glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
	return true;
}

/**
 * Tells OpenGL to use this shader program from now on.
 */
void Shader::use(){
	glUseProgram(this->id);
}

unsigned int Shader::getID(){
	return this->id;
}
