#include "imageread.h"
#include "Image.h"

#include <cstdint>
#include <cstdio>     
#include <fstream>    
#include <stdexcept>
#include <vector>
#include <string>
#include <memory>     

#include <png.h>

// Helper: RAII wrapper for FILE*
using FilePtr = std::unique_ptr<FILE, decltype(&std::fclose)>;

// Image type detection
std::string ImageReader::get_image_type(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);

    if (!file) {
        return "Couldn't open file.";
    }

    unsigned char header[8]{};
    file.read(reinterpret_cast<char*>(header), sizeof(header));

    if (file.gcount() < 4) {
        return "File maybe corrupt, or not an Image...";
    }

    if (header[0] == 0xFF && header[1] == 0xD8 && header[2] == 0xFF)
        return "JPEG";

    if (header[0] == 0x89 && header[1] == 'P' && header[2] == 'N' &&
        header[3] == 'G')
        return "PNG";

    if (header[0] == 'G' && header[1] == 'I' && header[2] == 'F')
        return "GIF";

    if (header[0] == 'B' && header[1] == 'M')
        return "BMP";

    return "Can't process this format";
}

// PNG reader
Image ImageReader::read_png(const std::string& filePath, Mode mode) {
    if (filePath.empty())
        throw std::invalid_argument("PNG reader: empty file path");

    FilePtr fp(std::fopen(filePath.c_str(), "rb"), std::fclose);
    if (!fp)
        throw std::runtime_error("PNG reader: failed to open file");

    png_byte signature[8];
    if (std::fread(signature, 1, 8, fp.get()) != 8 ||
        png_sig_cmp(signature, 0, 8)) {
        throw std::runtime_error("PNG reader: not a PNG file");
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png)
        throw std::runtime_error("png_create_read_struct failed");

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_read_struct(&png, nullptr, nullptr);
        throw std::runtime_error("png_create_info_struct failed");
    }

    struct PngGuard {
        png_structp png;
        png_infop info;
        ~PngGuard() {
            if (png)
                png_destroy_read_struct(&png, &info, nullptr);
        }
    } guard{ png, info };

    if (setjmp(png_jmpbuf(png))) {
        throw std::runtime_error("PNG reader: error during read");
    }

    png_init_io(png, fp.get());
    png_set_sig_bytes(png, 8);
    png_read_info(png, info);

    png_uint_32 width = png_get_image_width(png, info);
    png_uint_32 height = png_get_image_height(png, info);
    png_byte colorType = png_get_color_type(png, info);
    png_byte bitDepth = png_get_bit_depth(png, info);

    if (bitDepth == 16)
        png_set_strip_16(png);

    if (colorType == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);

    if (colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8)
        png_set_expand_gray_1_2_4_to_8(png);

    if (png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);

    // Convert color type according to mode requested
    // reading RGB (or PALETTE, which gets converted to RGB) as GRAY
    if (mode == Mode::GRAY &&
        (colorType == PNG_COLOR_TYPE_RGB ||
            colorType == PNG_COLOR_TYPE_RGB_ALPHA || colorType == PNG_COLOR_TYPE_PALETTE)) {
        png_set_rgb_to_gray(png, 1, -1, -1);
    }
    // reading GRAY as RGB 
    else if (mode == Mode::RGB && 
            (colorType == PNG_COLOR_TYPE_GRAY || 
             colorType == PNG_COLOR_TYPE_GRAY_ALPHA)) {
        png_set_gray_to_rgb(png);
    }

    png_read_update_info(png, info);

    colorType = png_get_color_type(png, info);

    std::size_t channels = 0;
    switch (colorType) {
    case PNG_COLOR_TYPE_GRAY:       channels = 1; break;
    case PNG_COLOR_TYPE_GRAY_ALPHA: channels = 2; break;
    case PNG_COLOR_TYPE_RGB:        channels = 3; break;
    case PNG_COLOR_TYPE_RGB_ALPHA:  channels = 4; break;
    default:
        throw std::runtime_error("Unsupported PNG color type");
    }

    Image img(height, width, channels);

    std::vector<png_bytep> rowPointers(height);
    const std::size_t rowBytes = width * channels;

    for (std::size_t y = 0; y < height; ++y) {
        rowPointers[y] =
            reinterpret_cast<png_bytep>(img.rawpixels() + y * rowBytes);
    }

    png_read_image(png, rowPointers.data());

    return img;
}