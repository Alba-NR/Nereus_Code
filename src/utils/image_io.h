#ifndef IMAGE_IO
#define IMAGE_IO
#pragma once


#include "stb/stb_image.h"

#include <string>

using std::string;


namespace ImageIO
{
	unsigned char *loadImage(string filename, int &width, int &height, int &num_channels, bool flip_vertically);
}

#endif
