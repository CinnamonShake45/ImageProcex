#ifndef IMAGE_H
#define IMAGE_H

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <vector>

class Image {
public:
	using iterator = std::vector<std::uint8_t>::iterator;
	using const_iterator = std::vector<std::uint8_t>::const_iterator;
	using pixel = std::uint8_t;
	using size = std::size_t;

	Image();
	Image(size rows, size columns, size channels);
	Image(Image &&) noexcept;
	Image(const Image &);
	Image &operator=(const Image &);
	~Image();

	pixel &at(size row, size col, size ch);
	const pixel &at(size row, size c, size ch) const;

	pixel *rawpixels() noexcept;
	const pixel *rawpixels() const noexcept;

	size getRows() const noexcept;
	size getColumns() const noexcept;
	size getChannels() const noexcept;

	// to make use of vector iterators for our image, useful for per-byte iterations
	// strictly for simple intensity transforms
	iterator begin() noexcept;
	iterator end() noexcept;
	const_iterator begin() const noexcept;
	const_iterator end() const noexcept; 

private:
	std::vector<std::uint8_t> data; // row, columns, channels format
	size rows;
	size columns;
	size channels;
	size index(size row, size col, size ch) const;
};

#endif /* IMAGE_H */
