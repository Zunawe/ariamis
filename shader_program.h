#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#ifndef OPENGL_HEADERS
#define OPENGL_HEADERS
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#endif

class ShaderProgram{
	public:
		bool init(const GLchar *vertexShaderPath, const GLchar *fragmentShaderPath);
		void use();
		unsigned int getID();
	
	private:
		unsigned int id;
		bool hasCompileErrors(unsigned int shaderID);
		bool hasLinkErrors();
};
#endif