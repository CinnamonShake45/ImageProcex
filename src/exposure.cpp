#include <algorithm>
#include <numeric>
#include <iostream>
#include <cmath>

#include "exposure.h"
#include "transforms.h"


void Exposure::get_bin_data(int bin_width, int& bin_count, std::vector<uint8_t>& bins,
    std::vector<float>& bin_centers) {

    if (bin_width < 2) {
        bin_width = 1;
        bin_centers = std::vector<float>(256);
        std::iota(bin_centers.begin(), bin_centers.end(), 0);
        bins = get_bins(bin_width, bin_count);
        return;
    }

    bin_count = 0;

    bins = get_bins(bin_width, bin_count);

    for (int i = 0; i < bin_count * 2; i += 2) {
        float center = (float)(bins[i] + bins[i + 1]) / 2;
        bin_centers.push_back(center);
    }

    return;
}

// DONT USE WHEN bin_width < 2, handle that in calling function (eg. get_histgram())
std::vector<uint8_t> Exposure::get_bins(int bin_width, int& bin_count) {
    std::vector<uint8_t> bins;

    // EDGE CASE, DON'T USE THIS
    if (bin_width < 2 ) {
        bins.reserve(512);
        for (int i = 0; i < 256; i++) {
            bins.push_back(i);
            bins.push_back(i);
        }
        bin_count = 256;
        return bins;
    }

    if (bin_width == 2) {
        bins.resize(256);
        std::iota(bins.begin(), bins.end(), 0);
        bin_width = 2;
        bin_count = 128;
        return bins;
    }

    bin_count = 255 / bin_width;
    int last_bin_width = 255 - bin_count * bin_width + 1;

    if (last_bin_width > (bin_width / 2))
        bin_count++;

    for (int i = 0, l_bin = 0, u_bin = bin_width - 1; i < bin_count * 2; i++) {
        if (i % 2 == 0) {
            bins.push_back(l_bin);
            l_bin += bin_width;
        }
        else {
            bins.push_back(u_bin);
            u_bin += bin_width;
        }
    }

    if (256 % bin_width != 0)
        bins[bin_count * 2 - 1] = 255;

    return bins;
}

Histogram<int> Exposure::get_histogram(const Image& img, int bin_width, int selected_channel) {
    int bin_count = 0;

    // Validate bin_width
    if (bin_width < 1) {
        bin_count = 256;
        bin_width = 1;
    }
    if (bin_width > 256)
        bin_width = 256;



}
/*

Histogram<int> Exposure::get_histogram(const Image& img, int bin_width, int selected_channel) {
    // Validate bin_width
    if (bin_width <= 0) bin_width = 1;
    int bin_count = 256;
    if (bin_width >= 256) bin_count = 1;

    size_t rows = img.getRows();
    size_t cols = img.getColumns();
    size_t img_channels = img.getChannels();

    std::vector<int> data(bin_count);

    if (bin_width < 2) {
        if (selected_channel == 0) {
            for (auto& px : img)
                data[px]++;
        }
        // selected_channel = 1 : Red, 2: Green, 3: Blue
        else { 
            
        }
    }

    else if (bin_width > 2){
        auto bins = get_bins(bin_width, bin_count);

    }

    auto bin_centers = get_bin_centers(bin_width);

    return {bin_count, std::move(data), std::move(bin_centers)};
} */