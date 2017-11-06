#ifndef OPENGL_HEADERS
#define OPENGL_HEADERS
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#endif

class Texture{
	public:
		Texture(const char *filename);
		unsigned int getID();

	private:
		unsigned int id;
		int width, height;
		int numChannels;
};