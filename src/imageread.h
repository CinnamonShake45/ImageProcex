#ifndef IMAGEREAD_H
#define IMAGEREAD_H

#include "Image.h"
#include <cstdint>
#include <string>

class ImageReader {
public:
  static std::string get_image_type(const char *filePath);
  static Image read_png(const char *filePath, std::uint8_t mode); // mode : 0 for grayscale, anything else for RGB
};

#endif
