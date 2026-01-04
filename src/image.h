#ifndef IMAGE_H
#define IMAGE_H

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <vector>

class image {
public:
  image();
  image(std::size_t rows, std::size_t columns, std::size_t channels);
  image(image &&) noexcept;
  image(const image &);
  image &operator=(image &&) noexcept;
  image &operator=(const image &);
  ~image();

  std::uint8_t &at(std::size_t row, std::size_t col, std::size_t ch);
  const uint8_t &at(std::size_t row, std::size_t c, std::size_t ch) const;

  std::uint8_t *rawpixels() noexcept;
  const std::uint8_t *rawpixels() const noexcept;

  std::size_t getRows() const noexcept;
  std::size_t getColumns() const noexcept;
  std::size_t getChannels() const noexcept;

private:
  std::vector<std::uint8_t> data; // row, columns, channels format
  std::size_t rows;
  std::size_t columns;
  std::size_t channels;
  std::size_t index(std::size_t row, std::size_t col, std::size_t ch) const;
};

#endif /* IMAGE_H */
