#ifndef IMAGEREAD_H
#define IMAGEREAD_H

#include <string>

class ImageReader {
public:
    static std::string getImageType(const std::string& filePath);
    std::vector<uint8_t> readPNG(const char* filepath, int& imageWidth, int& imageHeight);
};

#endif