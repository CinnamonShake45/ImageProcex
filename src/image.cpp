#include "image.h"
#include <cstddef>
#include <cstdint>
#include <stdexcept>

image::image() : rows(0), columns(0), channels(0) {};

image::image(std::size_t rows, std::size_t columns, std::size_t channels)
    : rows(rows), columns(columns), channels(channels),
      data(rows * columns * channels) {
  if (channels != 1 && channels != 3)
    throw std::invalid_argument(
        "image: channels can only be either 1 (grayscale) or 3 (color)");
}

image::~image() = default;
image::image(const image &) = default;
image::image(image &&) noexcept = default;
image &image::operator=(const image &) = default;

std::uint8_t &image::at(std::size_t row, std::size_t col, std::size_t ch) {
  return data[index(row, col, ch)];
}

const std::uint8_t &image::at(std::size_t row, std::size_t col,
                              std::size_t ch) const {
  return data[index(row, col, ch)];
}

std::uint8_t *image::rawpixels() noexcept { return data.data(); }

const std::uint8_t *image::rawpixels() const noexcept { return data.data(); }

std::size_t image::getRows() const noexcept { return rows; }

std::size_t image::getColumns() const noexcept { return columns; }

std::size_t image::getChannels() const noexcept { return channels; }

std::size_t image::index(std::size_t row, std::size_t col,
                         std::size_t ch) const {
  if (row >= rows || col >= columns || ch >= channels)
    throw std::out_of_range("image: index out of range");
  return (row * columns + col) * channels + ch;
};
