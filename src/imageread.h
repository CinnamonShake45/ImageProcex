#ifndef IMAGEREAD_H
#define IMAGEREAD_H

#include "image.h"
#include <cstdint>
#include <string>
#include <vector>
class ImageReader {
public:
  static std::string get_image_type(const char *filePath);
  static image
  read_png(const char *filepath,
           std::uint8_t mode); // mode : 0 for grayscale, anything else for RGB
};

#endif
