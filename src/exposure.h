#ifndef EXPOSURE_H
#define EXPOSURE_H

#include "Image.h"
#include <vector>

template <typename T> /* T: int (counts) or float (normalized) */
struct Histogram{
    int size;
    std::vector<T> data;
    std::vector<float> bin_centers;
};

class Exposure{
public:
    /* Helper for histogram binning. To be used ONLY when bin_width > 1 */
    static void get_bin_data(int bin_width, int& bin_count, std::vector<uint8_t>& bins, std::vector<float>& bin_centers);
    static std::vector<uint8_t> get_bins(int bin_width, int& bin_count);
    /* channel_selector: 0 for Flattened, 1 for Red, 2 for Green, 3 for Blue */
    static Histogram<int> get_histogram(const Image& img, int bin_width, int selected_channel); 
    static Histogram<float> normalize(const Histogram<int>& hist);
    static Image equalize(const Image& img);
private:
    Exposure() = delete;
};

#endif /* EXPOSURE_H */