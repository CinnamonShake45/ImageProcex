#ifndef IMAGEREAD_H
#define IMAGEREAD_H

#include "Image.h"
#include <cstdint>
#include <string>

enum class Mode{
  GRAY,
  RGB
};

class ImageReader {
public:
  static std::string get_image_type(const std::string& filePath);
  static Image read_png(const std::string& filePath, Mode mode); 
  static Image read_bmp(const std::string& filePath, Mode mode);
  static Image read_jpg(const std::string& filePath, Mode mode);

private:
    ImageReader() = delete;
};

#endif
