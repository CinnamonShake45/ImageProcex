#ifndef IMAGEREAD_H
#define IMAGEREAD_H

#include "Image.h"
#include <cstdint>
#include <string>

class ImageReader {
public:
  static std::string get_image_type(const char *filePath);
  // mode : 0 for grayscale, anything else for RGB
  static Image read_png(const char *filePath, std::uint8_t mode); 
  static Image read_bmp(const char* filePath, std::uint8_t mode);
  static Image read_jpg(const char* filePath, std::uint8_t mode);

private:
    ImageReader() = delete;
};

#endif
