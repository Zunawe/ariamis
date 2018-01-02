#include "texture.hpp"

Texture::Texture() : Texture("textures/default.png"){
	// Empty
}

Texture::Texture(const char *filePath){
	load(filePath);
}

/**
 * Loads an RGB PNG file into memory as a texture.
 * 
 * @param filePath the path to an RGB PNG file with 24-bit colors.
 */
void Texture::load(const char *filePath){
	stbi_set_flip_vertically_on_load(true);
	unsigned char *imageData = stbi_load(filePath, &width, &height, &numChannels, 3);

	glGenTextures(1, &id);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int Texture::getID() const{
	return id;
}
