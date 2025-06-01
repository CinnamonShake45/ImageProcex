#include <iostream>
#include "imageread.h"
#include <png.h>

int main() {
    std::string filePath;
    std::cout << "Enter the image file path: ";
    std::getline(std::cin, filePath);

    std::string type = ImageReader::getImageType(filePath);
    std::cout << "Identified type: " << type << std::endl;

    return 0;
}
