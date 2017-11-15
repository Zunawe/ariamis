#include "shader_program.hpp"

ShaderProgram::ShaderProgram(const GLchar *vertexShaderPath, const GLchar *fragmentShaderPath){
	// Read source code
	std::ifstream fileStream;
	fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	std::string vertexShaderSource;
	std::string fragmentShaderSource;

	try{
		fileStream.open(vertexShaderPath);

		std::stringstream ss;
		ss << fileStream.rdbuf();

		fileStream.close();
		vertexShaderSource = ss.str();
	}catch(std::ifstream::failure e){
		std::cout << "Failed to open file at " << vertexShaderPath << std::endl;
	}

	try{
		fileStream.open(fragmentShaderPath);

		std::stringstream ss;
		ss << fileStream.rdbuf();

		fileStream.close();
		fragmentShaderSource = ss.str();
	}catch(std::ifstream::failure e){
		std::cout << "Failed to open file at " << fragmentShaderPath << std::endl;
	}

	const char *vertexShaderSourceCStr = vertexShaderSource.c_str();
	const char *fragmentShaderSourceCStr = fragmentShaderSource.c_str();


	// Compile
	this->id = glCreateProgram();
	unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShaderID, 1, &vertexShaderSourceCStr, NULL);
	glCompileShader(vertexShaderID);
	if(hasCompileErrors(vertexShaderID)){
		std::cout << "Failed to compile vertex shader" << std::endl;
	}

	glShaderSource(fragmentShaderID, 1, &fragmentShaderSourceCStr, NULL);
	glCompileShader(fragmentShaderID);
	if(hasCompileErrors(fragmentShaderID)){
		std::cout << "Failed to compile fragment shader" << std::endl;
	}

	// Link
	glAttachShader(this->id, vertexShaderID);
	glAttachShader(this->id, fragmentShaderID);
	glLinkProgram(this->id);
	if(hasLinkErrors()){
		std::cout << "Failed to link shaders" << std::endl;
	}

}

bool ShaderProgram::hasCompileErrors(unsigned int shaderID){
	int success;
	char log[512];
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

	if(!success){
		glGetShaderInfoLog(this->id, sizeof(log), NULL, log);
		std::cout << "Shader compilation failed\n" << log << std::endl;
		return true;
	}
	return false;
}

bool ShaderProgram::hasLinkErrors(){
	int success;
	char log[512];
	glGetProgramiv(this->id, GL_LINK_STATUS, &success);

	if(!success){
		glGetShaderInfoLog(this->id, sizeof(log), NULL, log);
		std::cout << "Program linking failed\n" << log << std::endl;
		return true;
	}
	return false;
}

void ShaderProgram::use(){
	glUseProgram(this->id);
}

unsigned int ShaderProgram::getID(){
	return this->id;
}
