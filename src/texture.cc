#include <ariamis/texture.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Ariamis {
	Texture::Texture(){
		glGenTextures(1, &this->id);
		unsigned char whitePixel[3] = {255, 255, 255};
		loadRaw(1, 1, whitePixel);
	}

	Texture::Texture(const char *filePath){
		glGenTextures(1, &this->id);
		load(filePath);
	}

	Texture::~Texture(){
		if(glfwGetCurrentContext() != NULL)
			glDeleteTextures(1, &id);
	}

	/**
	* Loads an RGB PNG file into memory as a texture.
	* 
	* @param filePath the path to an RGB PNG file with 24-bit colors.
	*/
	void Texture::load(const char *filePath){
		stbi_set_flip_vertically_on_load(true);
		unsigned char *imageData = stbi_load(filePath, &width, &height, &numChannels, 3);

		loadToGPU(imageData);

		delete imageData;
	}

	void Texture::loadRaw(unsigned int width, unsigned int height, unsigned char *data){
		this->width = width;
		this->height = height;
		loadToGPU(data);
	}

	/**
	* Sends an array of pixels to the GPU as a texture and saves its name.
	* 
	* @param imageData raw pixel data.
	*/
	void Texture::loadToGPU(unsigned char *imageData){
		glBindTexture(GL_TEXTURE_2D, id);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
			glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	unsigned int Texture::getID() const{
		return id;
	}
}
