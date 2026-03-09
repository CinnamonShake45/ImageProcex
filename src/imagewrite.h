#ifndef IMAGEWRITE_H
#define IMAGEWRITE_H

#include "Image.h"
#include <cstdint>
#include <string>

class ImageWriter {
public:
	static void write_png(const char* filePath, const Image& img);
};

#endif
