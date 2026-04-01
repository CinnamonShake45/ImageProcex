#include <numeric>

#include "exposure.h"

void Exposure::get_bin_data(int bin_width, int& bin_count, std::vector<uint8_t>& bins,
    std::vector<float>& bin_centers) {

    if (bin_width < 2) {
        bin_width = 1;
        bin_centers = std::vector<float>(256);
        std::iota(bin_centers.begin(), bin_centers.end(), 0);
        bins = get_bins(bin_width, bin_count);
        bin_count = 256;
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

// DONT USE DIRECTLY! Helper for get_bin_data
std::vector<uint8_t> Exposure::get_bins(int bin_width, int& bin_count) {
    std::vector<uint8_t> bins;

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
    
    // Validate bin_width - handle bad input for bin_width
    // if less than 1, make it 1
    if (bin_width < 1) {
        bin_count = 256;
        bin_width = 1;
    }
    // if greater than 256, make it 256
    if (bin_width > 256){
        bin_width = 256;
        bin_count = 1;
    }

    if (selected_channel > 3 || selected_channel < 0)
        selected_channel = 0;

    size_t row_count = img.getRows(), column_count = img.getColumns(), channel_count = img.getChannels();

    if (img.getChannels() == 1) {

    }

    std::vector<int> data;
    std::vector<float> bin_centers;
    std::vector<uint8_t> bins;

    get_bin_data(bin_width, bin_count, bins, bin_centers);

    data = std::vector<int>(bin_count, 0);

    if (selected_channel == 0)  { 
        if (bin_width < 2) {
            for(auto& px : img){
                data[px]++;
            }
        }
        else {
            for (auto& px : img) {
                for (int i = 0, b = 0; i < bin_count; i++, b++) {
                    if(px >= bins[b] && px <= bins[++b])
                        data[i]++;
                }
            }
        }
    }

    if (selected_channel > 0) {
        int channel = selected_channel - 1;
        if (bin_width < 2) {
            for (size_t r = 0; r < row_count; r++) {
                for (size_t c = 0; c < row_count; c++) {
                    data[img.at(r, c, channel)]++;
                }
            }
        }
        else {
            for (size_t r = 0; r < row_count; r++) {
                for (size_t c = 0; c < row_count; c++) {
                    for(int i = 0, b = 0; i < bin_count; i++, b++) {
                        auto px = img.at(r, c, channel);
                        if (px >= bins[b] && px <= bins[++b]) {
                            data[i]++;
                        }
                    }
                }
            }
        }
    }

    return { bin_count, std::move(data), std::move(bin_centers)};      
} 
 