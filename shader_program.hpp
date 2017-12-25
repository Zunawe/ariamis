#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class ShaderProgram{
	public:
		static ShaderProgram DEFAULT_SHADER;

		void loadSources(const GLchar *vertexShaderPath, const GLchar *fragmentShaderPath);
		void use();
		unsigned int getID();
	
	private:
		unsigned int id;
		bool hasCompileErrors(unsigned int shaderID);
		bool hasLinkErrors();
};

#endif
