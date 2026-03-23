#include "imagewrite.h"

#include <png.h>
#include <cstdio>
#include <stdexcept>
#include <string>
#include <memory>
#include <cstdint>

// RAII wrapper for FILE*
using FilePtr = std::unique_ptr<FILE, decltype(&std::fclose)>;

void ImageWriter::write_png(const std::string& filePath, const Image& img)
{
    if (filePath.empty())
        throw std::invalid_argument("PNG Writer: empty file path");

    FilePtr fp(std::fopen(filePath.c_str(), "wb"), std::fclose);
    if (!fp)
        throw std::runtime_error("PNG Writer: Failed to open file for writing");

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png)
        throw std::runtime_error("PNG Writer: png_create_write_struct failed");

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_write_struct(&png, nullptr);
        throw std::runtime_error("png_create_info_struct failed");
    }

    // RAII guard for libpng cleanup
    struct PngGuard {
        png_structp png;
        png_infop info;
        ~PngGuard() {
            if (png)
                png_destroy_write_struct(&png, &info);
        }
    } guard{ png, info };

    if (setjmp(png_jmpbuf(png))) {
        throw std::runtime_error("Error during PNG creation");
    }

    png_init_io(png, fp.get());

    int width = img.getColumns();
    int height = img.getRows();
    int channels = img.getChannels();

    int color_type;
    if (channels == 1)
        color_type = PNG_COLOR_TYPE_GRAY;
    else if (channels == 3)
        color_type = PNG_COLOR_TYPE_RGB;
    else
        throw std::runtime_error("PNG Writer: Unsupported channel count");

    png_set_IHDR(
        png,
        info,
        width,
        height,
        8,
        color_type,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );

    png_write_info(png, info);

    const std::uint8_t* data = img.rawpixels();
    std::size_t row_bytes = width * channels;

    for (int y = 0; y < height; ++y)
    {
        png_write_row(png,
            reinterpret_cast<png_bytep>(const_cast<std::uint8_t*>(data + y * row_bytes)));
    }

    png_write_end(png, nullptr);
}