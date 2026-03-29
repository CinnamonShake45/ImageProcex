#ifndef EXPOSURE_H
#define EXPOSURE_H

#include "Image.h"
#include <array>

enum class histogramMode{
    GRAY,
    RGB
};

class Exposure{
private:
    std::array<int, 256> histogram;
public:
    static std::array<int, 256> get_histogram(const Image& img, histogramMode m);
    static std::array<float, 256> normalize(const Image& img);
    static Image equalize(const Image& img);
};

#endif /* EXPOSURE_H */