#ifndef IMAGEREAD_H
#define IMAGEREAD_H

#include <string>

class ImageReader {
public:
    static std::string getImageType(const std::string& filePath);
};

#endif