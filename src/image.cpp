#include "image.h"
#include <cstdint>

image::image() {
  imageRows = 0;
  imageColumns = 0;
  imageChannels = 0;
  pixelData = nullptr;
}

image::image(std::uint64_t imageRows, std::uint64_t imageColumns,
             std::uint8_t imageChannels) {
  this->imageRows = imageRows;
  this->imageColumns = imageColumns;
  this->imageChannels = imageChannels;

  pixelData = new std::uint8_t **[imageRows];
  for (std::size_t i = 0; i < imageRows; i++) {
    pixelData[i] = new std::uint8_t *[imageColumns];
    for (std::size_t j = 0; j < imageColumns; j++) {
      pixelData[i][j] = new std::uint8_t[imageChannels];
      for (std::uint8_t k = 0; k < imageChannels; k++) {
        pixelData[i][j][k] = 0;
      }
    }
  }
}

image::~image() {
  imageRows = 0;
  imageColumns = 0;
  imageChannels = 0;
  delete pixelData;
}
