#include "Image.h"
#include <cstddef>
#include <cstdint>
#include <stdexcept>

Image::Image() : rows(0), columns(0), channels(0) {};

Image::Image(std::size_t rows, std::size_t columns, std::size_t channels)
    : rows(rows), columns(columns), channels(channels),
      data(rows * columns * channels) {
  if (channels != 1 && channels != 3)
    throw std::invalid_argument(
        "Image: channels can only be either 1 (grayscale) or 3 (color)");
}

Image::~Image() = default;
Image::Image(const Image &) = default;
Image::Image(Image &&) noexcept = default;
Image &Image::operator=(const Image &) = default;

std::uint8_t &Image::at(std::size_t row, std::size_t col, std::size_t ch) {
  return data[index(row, col, ch)];
}

const std::uint8_t &Image::at(std::size_t row, std::size_t col,
                              std::size_t ch) const {
  return data[index(row, col, ch)];
}

std::uint8_t *Image::rawpixels() noexcept { return data.data(); }

const std::uint8_t *Image::rawpixels() const noexcept { return data.data(); }

std::size_t Image::getRows() const noexcept { return rows; }

std::size_t Image::getColumns() const noexcept { return columns; }

std::size_t Image::getChannels() const noexcept { return channels; }

std::size_t Image::index(std::size_t row, std::size_t col,
                         std::size_t ch) const {
  if (row >= rows || col >= columns || ch >= channels)
    throw std::out_of_range("Image: index out of range");
  return (row * columns + col) * channels + ch;
};
