#ifndef TEXTURE_H
#define TEXTURE_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "stb_image.h"

class Texture{
	public:
		Texture();
		Texture(const char *filePath);
		void load(const char *filename);
		void load();
		unsigned int getID();

	private:
		std::string filePath;
		unsigned int id;
		int width, height;
		int numChannels;
};

const Texture DEFAULT_TEXTURE;

#endif
