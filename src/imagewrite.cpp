#include "imagewrite.h"
#include <png.h>
#include <cstdio>
#include <stdexcept>

void ImageWriter::write_png(const char* filePath, const Image& img)
{
    // creating a file
    FILE* fp = fopen(filePath, "wb");
    if (!fp)
        throw std::runtime_error("PNG Writer: Failed to open file for writing");

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png)
    {
        fclose(fp);
        throw std::runtime_error("PNG Writer: png_create_write_struct failed");
    }

    png_infop info = png_create_info_struct(png);
    if (!info)
    {
        png_destroy_write_struct(&png, nullptr);
        fclose(fp);
        throw std::runtime_error("png_create_info_struct failed");
    }

    if (setjmp(png_jmpbuf(png)))
    {
        png_destroy_write_struct(&png, &info);
        fclose(fp);
        throw std::runtime_error("Error during PNG creation");
    }

    png_init_io(png, fp);

    // initializing important image info
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

    // writing the image data directly into the png file
    const uint8_t* data = img.rawpixels();

    size_t row_bytes = width * channels;

    for (int y = 0; y < height; y++)
    {
        png_write_row(png, (png_bytep)(data + y * row_bytes));
    }

    png_write_end(png, nullptr);

    png_destroy_write_struct(&png, &info);

    fclose(fp);
}