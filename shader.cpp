#include "shader.h"

using namespace std;

Shader::Shader(const char *filePath, GLenum shaderType){
	this->shaderType = shaderType;
	this->id = glCreateShader(shaderType);

	ifstream fileStream;
	fileStream.exceptions(ifstream::failbit | ifstream::badbit);

	try{
		fileStream.open(filePath);

		stringstream ss;
		ss << fileStream.rdbuf();
		fileStream.close();

		source = ss.str();
	} catch(ifstream::failure e){
		cout << "Failed to open file at " << filePath << endl;
	}
}

Shader::~Shader(){
	glDeleteShader(this->id);
}

unsigned int Shader::getID(){
	return this->id;
}

GLenum Shader::getShaderType(){
	return this->shaderType;
}

bool Shader::compile(){
	const char *sourceCStr = (this->source).c_str();
	glShaderSource(this->id, 1, &sourceCStr, NULL);
	glCompileShader(this->id);
	checkCompileErrors();
}

bool Shader::checkCompileErrors(){
	int success;
	char log[512];
	glGetShaderiv(this->id, GL_COMPILE_STATUS, &success);

	if(!success){
		glGetShaderInfoLog(this->id, sizeof(log), NULL, log);
		cout << "Shader compilation failed\n" << log << endl;
	}
}