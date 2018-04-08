#ifndef TEXTURE_H
#define TEXTURE_H

#include <glm/glm.hpp>

class Texture{
	public:
		Texture();
		~Texture();
		Texture(const char *filePath);
		void load(const char *filename);
		unsigned int getID() const;

	private:
		void generateTexture(unsigned char *imageData);

		unsigned int id;
		int width, height;
		int numChannels;
};

#endif
