#ifndef IMAGE_H
#define IMAGE_H

#include <cstdint>

class image {
public:
  std::uint8_t ***pixelData; // row, columns, channels
  std::size_t imageRows;
  std::size_t imageColumns;
  std::uint8_t imageChannels;

  image();
  image(uint64_t imageRows, uint64_t imageColumns, uint8_t imageChannels);
  image(image &&) = default;
  image(const image &) = default;
  image &operator=(image &&) = default;
  image &operator=(const image &) = default;
  ~image();
};

#endif /* IMAGE_H */
