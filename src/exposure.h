#ifndef EXPOSURE_H
#define EXPOSURE_H

#include "Image.h"
#include <vector>

enum class HistogramMode{
    GRAYSCALE, /* RGB Image converted to GRAYSCALE then histogram returned (vector of size 1)*/
    FLATTENED, /* Flatten channels then histogram returned (vector of size 1)*/
    PER_CHANNEL /* If Image is RGB then per channel histogram returned (vector of size 3) */
};

template <typename T> /* T: int (counts) or float (normalized) */
struct Histogram{
    std::vector<std::vector<T>> data; /* data[channel][bin_index] */
    std::vector<float> bin_centers;
};

class Exposure{
public:
    static std::vector<float> get_bin_centers(int bin_count, int bin_width);
    static Histogram<int> get_histogram(Image img, HistogramMode mode, int bin_width); 
    static Histogram<float> normalize(const Histogram<int>& hist);
    static Image equalize(const Image& img);
private:
    Exposure() = delete;
};

#endif /* EXPOSURE_H */