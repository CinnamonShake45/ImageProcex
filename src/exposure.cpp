#include <algorithm>
#include <numeric>
#include <iostream>
#include <cmath>

#include "exposure.h"
#include "transforms.h"

std::vector<float> Exposure::get_bin_centers(int bin_count, int bin_width){
    std::vector<float> bin_centers(bin_count);
    for (int i = 0; i < bin_count; ++i) {
        int left = i * bin_width;
        int right = std::min((i + 1) * bin_width - 1, 255);
        bin_centers[i] = (left + right) / 2.0f;
    }
    return bin_centers;
}

Histogram<int> Exposure::get_histogram(Image img, HistogramMode mode, int bin_width) {
    // Validate bin_width
    if (bin_width <= 0) bin_width = 1;
    
    int bin_count = (255 + bin_width) / bin_width;   
    if (bin_width >= 256) bin_count = 1;
    
    size_t out_channels = 1;
    if (mode == HistogramMode::PER_CHANNEL) {
        out_channels = img.getChannels();
    }
    
    // Initialize data vector: out_channels vectors each of size bin_count, filled with 0
    std::vector<std::vector<int>> data(out_channels, std::vector<int>(bin_count, 0));
    
    size_t rows = img.getRows();
    size_t cols = img.getColumns();
    size_t ch = img.getChannels();

    std::vector<float> bin_centers = get_bin_centers(bin_count, bin_width);
    
    return {std::move(data), std::move(bin_centers)};
}