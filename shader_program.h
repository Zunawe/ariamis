#include <iostream>

#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#ifndef OPENGL_HEADERS
#define OPENGL_HEADERS
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#endif

#include "shader.h"

class ShaderProgram{
	public:
		ShaderProgram();
		void attachShader(Shader shader);
		bool link();
		void use();
	
	private:
		unsigned int id;
		bool checkLinkErrors();
};
#endif
