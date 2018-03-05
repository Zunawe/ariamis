#ifndef TEXTURE_H
#define TEXTURE_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Texture{
	public:
		Texture();
		Texture(const char *filePath);
		void load(const char *filename);
		unsigned int getID() const;

	private:
		unsigned int id;
		int width, height;
		int numChannels;

		void generateTexture(unsigned char *imageData);
};

#endif
