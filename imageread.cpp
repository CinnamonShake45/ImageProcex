#include "imageread.h"

#include <stdint.h>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <vector>

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

std::vector<uint8_t> ImageReader::readPNG(const char* filepath, int& imageWidth, int& imageHeight){
    FILE *fp = fopen(filepath, "rb");
    if(!fp) 
        throw std::runtime_error("Failed to open file");


    png_byte pngHeader[8];
    fread(pngHeader, 1, 8, fp);

    bool is_png =!png_sig_cmp(pngHeader, 0, 8);
    if (!is_png){
        fclose(fp);
        throw std::runtime_error("Not a PNG file");
    }
    
    png_structp pngReadPointer = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if(!pngReadPointer)
        throw std::runtime_error("PNG read struct allocation error");


    png_infop pngInfoPointer = png_create_info_struct(pngReadPointer);
    if(!pngInfoPointer){
        png_destroy_read_struct(&pngReadPointer, NULL, NULL);
        throw std::runtime_error("PNG info struct allocation error");
    }

    if(setjmp(png_jmpbuf(pngReadPointer))){
        png_destroy_read_struct(&pngReadPointer, &pngInfoPointer, NULL);
        fclose(fp);
        throw std::runtime_error("Libpng read error");
    }

    png_init_io(pngReadPointer, fp);
    png_set_sig_bytes(pngReadPointer, 8);
    png_read_info(pngReadPointer, pngInfoPointer);

    imageWidth = png_get_image_width(pngReadPointer, pngInfoPointer);
    imageHeight = png_get_image_height(pngReadPointer, pngInfoPointer);
    png_byte imageColorType = png_get_color_type(pngReadPointer, pngInfoPointer);
    png_byte imageBitDepth = png_get_bit_depth(pngReadPointer, pngInfoPointer);

    if(imageBitDepth == 16)
        png_set_strip_16(pngReadPointer);
    
    if(imageColorType == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(pngReadPointer);

    if(imageColorType == PNG_COLOR_TYPE_GRAY && imageBitDepth < 8)
        png_set_expand_gray_1_2_4_to_8(pngReadPointer);

    if(png_get_valid(pngReadPointer, pngInfoPointer, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(pngReadPointer);
    
    if(imageColorType == PNG_COLOR_TYPE_GRAY || imageColorType == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(pngReadPointer);

    png_set_strip_alpha(pngReadPointer);

    png_read_update_info(pngReadPointer, pngInfoPointer);

    size_t rowBytes = png_get_rowbytes(pngReadPointer, pngInfoPointer);
    std::vector<uint8_t> imageData(imageHeight  * rowBytes);

    std::vector<png_bytep> rowPointers(imageHeight);
    for(int y = 0; y < imageHeight; y++)
        rowPointers[y] = &imageData[y * rowBytes];
    
        png_read_image(pngReadPointer ,rowPointers.data());
    png_destroy_read_struct(&pngReadPointer, &pngInfoPointer, NULL);
    fclose(fp);
    return imageData;
}

