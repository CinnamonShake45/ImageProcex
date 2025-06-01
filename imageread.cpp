#include "imageread.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <png.h>

std::string ImageReader::getImageType(const std::string& filePath){
    std::ifstream file(filePath, std::ios::binary);

    if (!file) {
        return "Couldn't open file.";
    }

    std::vector<unsigned char> header(8);
    file.read(reinterpret_cast<char*>(header.data()),header.size());

    if(file.gcount() < 4) {
        return "File maybe corrupt, or not an image...";
    }

    if (header[0] == 0xFF && header[1] == 0xD8 && header[2] == 0xFF) 
        return "JPEG";
    
    if (header[0] == 0x89 && header[1] == 'P' && header[2] == 'N' && header[3] == 'G') 
        return "PNG";

    if (header[0] == 'G' && header[1] == 'I' && header[2] == 'F') 
        return "GIF";
    
    if (header[0] == 'B' && header[1] == 'M') 
        return "BMP";
    
    return "Can't process this format";
}