#include "shader_program.h"

ShaderProgram::ShaderProgram(){
	this->id = glCreateProgram();
}

void ShaderProgram::attachShader(Shader shader){
	glAttachShader(this->id, shader.getID());
}

bool ShaderProgram::link(){
	glLinkProgram(this->id);
	return checkLinkErrors();
}

bool ShaderProgram::checkLinkErrors(){
	int success;
	char log[512];
	glGetProgramiv(this->id, GL_LINK_STATUS, &success);

	if(!success){
		glGetShaderInfoLog(this->id, sizeof(log), NULL, log);
		std::cout << "Program linking failed\n" << log << std::endl;
		return false;
	}
	return true;
}

void ShaderProgram::use(){
	glUseProgram(this->id);
}

unsigned int ShaderProgram::getID(){
	return id;
}
