#include "image_io.h"

#include <iostream>

string TEXTURES_FOLDER_PATH = PROJECT_SOURCE_DIR "/resources/textures/";


unsigned char *ImageIO::loadImage(string filename, int &width, int &height, int &num_channels)
{
	string filepath = TEXTURES_FOLDER_PATH + filename;
	
	unsigned char *img_data = stbi_load(filepath.c_str(), &width, &height, &num_channels, 0);

	// TODO chck if img_data == NULL & throw exception? (could not load file ok)
	if (img_data == NULL)
	{
		std::cout << stbi_failure_reason() << std::endl;
		// TODO throw exception
	}

	return img_data;
}
