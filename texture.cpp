#include "texture.hpp"

Texture::Texture(){
	this->filePath = "textures/default.png";
}

Texture::Texture(const char *filePath){
	this->filePath = std::string(filePath);
}

void Texture::load(){
	load(filePath.c_str());
}

/**
 * Loads an RGB PNG file into memory as a texture.
 * 
 * @param filePath the path to an RGB PNG file with 24-bit colors.
 */
void Texture::load(const char *filePath){
	std::vector<unsigned char> imageData;
	std::vector<unsigned char> fileBuffer;

	std::ifstream file;
	std::streamsize size = 0;
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try{
		file.open(filePath);

		if(file.seekg(0, std::ios::end).good()){
			size = file.tellg();
		}
		if(file.seekg(0, std::ios::beg).good()){
			size -= file.tellg();
		}

		fileBuffer.resize((size_t)size);
		file.read((char*)(&fileBuffer[0]), size);

		file.close();
	}catch(std::ifstream::failure e){
		std::cout << "Failed to open file at " << filePath << std::endl;
	}

	unsigned long w, h;
	decodePNG(imageData, w, h, &fileBuffer[0], (unsigned long)fileBuffer.size(), true);
	width = (unsigned int)w;
	height = (unsigned int)h;
	glGenTextures(1, &id);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id);
	if(numChannels == 3){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, &imageData[0]);
	}
	else{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &imageData[0]);
	}
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int Texture::getID(){
	return id;
}
