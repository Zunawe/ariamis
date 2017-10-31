#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#ifndef SHADER_H
#define SHADER_H

#ifndef OPENGL_HEADERS
#define OPENGL_HEADERS
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#endif

class Shader{
	public:
		Shader(const char *filePath, GLenum shaderType);
		~Shader();

		unsigned int getID();
		bool compile();
	
	private:
		unsigned int id;
		std::string source;
		bool checkCompileErrors();
};
#endif
