#include "Image.h"
#include <cstddef>
#include <cstdint>
#include <stdexcept>

Image::Image() : rows(0), columns(0), channels(0) {};

Image::Image(Image::size rows, Image::size columns, Image::size channels)
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

Image::pixel &Image::at(Image::size row, Image::size col, Image::size ch) {
  return data[index(row, col, ch)];
}

const Image::pixel &Image::at(Image::size row, Image::size col,
                              Image::size ch) const {
  return data[index(row, col, ch)];
}

Image::pixel *Image::rawpixels() noexcept { return data.data(); }

const Image::pixel *Image::rawpixels() const noexcept { return data.data(); }

Image::size Image::getRows() const noexcept { return rows; }

Image::size Image::getColumns() const noexcept { return columns; }

Image::size Image::getChannels() const noexcept { return channels; }

Image::size Image::index(Image::size row, Image::size col,
                         Image::size ch) const {
  if (row >= rows || col >= columns || ch >= channels)
    throw std::out_of_range("Image: index out of range");
  return (row * columns + col) * channels + ch;
};

Image::iterator Image::begin() noexcept { return data.begin(); }
Image::iterator Image::end() noexcept { return data.end(); }
Image::const_iterator Image::begin() const noexcept { return data.begin(); }
Image::const_iterator Image::end() const noexcept { return data.end(); }