#include <iostream>
#include <vector>
#include <stdint.h>

#include <png.h>

#include "imageread.h"

int main() {
    const char* filePath = "C:/Users/Shikhar Dixit/Self Projects/ImageProcex/test-stuff/bigImage.png";

    std::string type = ImageReader::get_image_type(filePath);
    std::cout << "Identified type: " << type << std::endl;

    if(type != "PNG"){
        std::cout << "Please input only a .png file (for now)" << std::endl;
        return 0;
    }

    int width, height;

    std::vector<uint8_t> imagedata;

    try{    
        if(type == "PNG")
            imagedata = ImageReader::read_png_file(filePath, width, height, 0);
    }
    catch (const char* error){
        std::cout << error << std::endl;
    }

    for(int y = 0; y < width; ++y){
        for(int x = 0; x < height; ++x){
            const uint8_t* pixel = &imagedata[(y * width + x) * 1];
            std::cout << (int)pixel[0] << "\t";
        }
        std::cout << "\n";
    }

    return 0;
}
