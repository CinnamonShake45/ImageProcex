#include "imageread.h"
#include "image.h"
#include <cstdint>
#include <cstdio>
#include <stdexcept>
#include <stdint.h>
#include <vector>

#include <png.h>

std::string ImageReader::get_image_type(const char *filePath) {
  FILE *fp = fopen(filePath, "rb");

  if (!fp) {
    return "Couldn't open file.";
  }

  unsigned char header[8];
  size_t bytes_read = std::fread(header, 1, sizeof(header), fp);
  fclose(fp);

  if (bytes_read < 4) {
    return "File maybe corrupt, or not an image...";
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

image ImageReader::read_png(const char *filePath, std::uint8_t mode) {
  if (!filePath)
    throw std::invalid_argument("PNG reader: null file path");

  FILE *fp = std::fopen(filePath, "rb");
  if (!fp)
    throw std::runtime_error("PNG reader: failed to open file");

  // Validate PNG signature
  png_byte signature[8];
  if (std::fread(signature, 1, 8, fp) != 8 || png_sig_cmp(signature, 0, 8)) {
    std::fclose(fp);
    throw std::runtime_error("PNG reader: not a PNG file");
  }

  png_structp png =
      png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  if (!png) {
    std::fclose(fp);
    throw std::runtime_error("png_create_read_struct failed");
  }

  png_infop info = png_create_info_struct(png);
  if (!info) {
    png_destroy_read_struct(&png, nullptr, nullptr);
    std::fclose(fp);
    throw std::runtime_error("png_create_info_struct failed");
  }

  if (setjmp(png_jmpbuf(png))) {
    png_destroy_read_struct(&png, &info, nullptr);
    std::fclose(fp);
    throw std::runtime_error("PNG reader: error during read");
  }

  png_init_io(png, fp);
  png_set_sig_bytes(png, 8);
  png_read_info(png, info);

  png_uint_32 width = png_get_image_width(png, info);
  png_uint_32 height = png_get_image_height(png, info);
  png_byte colorType = png_get_color_type(png, info);
  png_byte bitDepth = png_get_bit_depth(png, info);

  // normalizing to 8-bits

  if (bitDepth == 16)
    png_set_strip_16(png);

  if (colorType == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png);

  if (colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8)
    png_set_expand_gray_1_2_4_to_8(png);

  if (png_get_valid(png, info, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png);

  // determine if user wants to read in grayscale only

  if (mode == 0 && (colorType == PNG_COLOR_TYPE_RGB ||
                    colorType == PNG_COLOR_TYPE_RGB_ALPHA)) {
    png_set_rgb_to_gray_fixed(png, 1, -1, -1);
  }

  png_read_update_info(png, info);

  // determining final channel count
  colorType = png_get_color_type(png, info);

  std::size_t channels = 0;
  switch (colorType) {
  case PNG_COLOR_TYPE_GRAY:
    channels = 1;
    break;
  case PNG_COLOR_TYPE_GRAY_ALPHA:
    channels = 2;
    break;
  case PNG_COLOR_TYPE_RGB:
    channels = 3;
    break;
  case PNG_COLOR_TYPE_RGB_ALPHA:
    channels = 4;
    break;
  default:
    png_destroy_read_struct(&png, &info, nullptr);
    std::fclose(fp);
    throw std::runtime_error("Unsupported PNG color type");
  }

  image img(height, width, channels);

  // read the image directly into the vector

  std::vector<png_bytep> rowPointers(height);
  const std::size_t rowBytes = width * channels;

  for (std::size_t y = 0; y < height; ++y) {
    rowPointers[y] =
        reinterpret_cast<png_bytep>(img.rawpixels() + y * rowBytes);
  }

  png_read_image(png, rowPointers.data());

  png_destroy_read_struct(&png, &info, nullptr);
  std::fclose(fp);

  return img;
}
