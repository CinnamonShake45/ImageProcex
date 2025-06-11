#include "imageread.h"

#include <stdint.h>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <vector>

#include <png.h>

std::string ImageReader::get_image_type(const char* filePath){
    FILE* fp = fopen(filePath, "rb");

    if (!fp) {
        return "Couldn't open file.";
    }

    unsigned char header[8];
    size_t bytes_read = std::fread(header, 1, sizeof(header), fp);

    if(bytes_read < 4) {
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

std::vector<uint8_t> ImageReader::read_png_file(const char* filepath, int& width, int& height, int mode){
    
    if(mode!=0 && mode!=1)
        throw std::invalid_argument("mode must be 0 (grayscale) or 1 (RGB)");

    FILE *fp = fopen(filepath, "rb");

    if(!fp) 
        throw std::runtime_error("Failed to open file");

    png_byte header[8];

    fread(header, 1, 8, fp);
    bool is_png =!png_sig_cmp(header, 0, 8);
    if (!is_png){
        fclose(fp);
        throw std::runtime_error("Not a PNG file");
    }
    
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if(!png_ptr)
        throw std::runtime_error("PNG read struct allocation error");

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if(!info_ptr){
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        throw std::runtime_error("PNG info struct allocation error");
    }

    if(setjmp(png_jmpbuf(png_ptr))){
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        throw std::runtime_error("Libpng read error");
    }

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);

    width = png_get_image_width(png_ptr, info_ptr);
    height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    if(bit_depth == 16)
        png_set_strip_16(png_ptr);
    
    if(color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png_ptr);

    if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png_ptr);

    if(png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png_ptr);

    if(mode == 1) {
        if(color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
            png_set_gray_to_rgb(png_ptr);
    } else {
        if(color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_RGB_ALPHA)
            png_set_rgb_to_gray_fixed(png_ptr, 1, -1, -1);
        
    }
    
    png_set_strip_alpha(png_ptr);

    png_read_update_info(png_ptr, info_ptr);

    size_t rowBytes = png_get_rowbytes(png_ptr, info_ptr);
    std::vector<uint8_t> image_data(height  * rowBytes);

    std::vector<png_bytep> rowPointers(height);
    for(size_t y = 0; y < height; y++)
        rowPointers[y] = &image_data[y * rowBytes];
    
        png_read_image(png_ptr ,rowPointers.data());
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    fclose(fp);

    return image_data;
}

