#ifndef IMAGEREAD_H
#define IMAGEREAD_H

#include <string>
#include <vector>

class ImageReader {
public:
    static std::string get_image_type(const char* filePath);
    static std::vector<uint8_t> read_png_file(const char* filepath, int& imageWidth, int& imageHeight, int mode);
};

#endif