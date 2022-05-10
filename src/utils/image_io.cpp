#define STB_IMAGE_IMPLEMENTATION
#include "image_io.h"

#include <iostream>

string TEXTURES_FOLDER_PATH = PROJECT_SOURCE_DIR "/resources/textures/";


unsigned char *ImageIO::loadImage(string filename, int &width, int &height, int &num_channels, bool flip_vertically)
{
	if (flip_vertically) stbi_set_flip_vertically_on_load(true);

	string filepath = TEXTURES_FOLDER_PATH + filename;
	
	unsigned char *img_data = stbi_load(filepath.c_str(), &width, &height, &num_channels, 0);

	// check if could not load file ok (img_data == NULL)
	if (img_data == NULL)
	{
		std::cout << "ERROR::IMAGE_IO::COULD_NOT_OPEN_FILE" << std::endl
			<< "File '" << filename << "' not found" << std::endl
			<< "STBI failure reason: " << stbi_failure_reason() << std::endl;
	}

	return img_data;
}
