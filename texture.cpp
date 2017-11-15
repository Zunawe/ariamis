#include "texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const char *filename){
	unsigned char *imageData = stbi_load(filename, &width, &height, &numChannels, 0);
	glGenTextures(1, &id);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id);
	if(numChannels == 3){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
	}
	else{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);	
	}
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(imageData);
}

unsigned int Texture::getID(){
	return id;
}