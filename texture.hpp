#ifndef TEXTURE_H
#define TEXTURE_H

#ifndef OPENGL_HEADERS
#define OPENGL_HEADERS
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#endif

class Texture{
	public:
		void load(const char *filename);
		unsigned int getID();

	private:
		unsigned int id;
		int width, height;
		int numChannels;
};

#endif