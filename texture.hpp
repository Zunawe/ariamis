#ifndef TEXTURE_H
#define TEXTURE_H

#include <vector>
#include <iostream>
#include <fstream>

#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "picopng.hpp"

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