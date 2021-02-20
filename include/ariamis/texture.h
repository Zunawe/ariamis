#ifndef TEXTURE_H
#define TEXTURE_H

namespace Ariamis {
	class Texture{
		public:
			Texture();
			Texture(const char *filePath);
			~Texture();
			void load(const char *filePath);
			void loadRaw(unsigned int width, unsigned int height, unsigned char *data);
			unsigned int getID() const;

		private:
			void loadToGPU(unsigned char *imageData);

			unsigned int id;
			int width, height;
			int numChannels;
	};
}

#endif
