#include <iostream>

#include "transforms.h"

Image Transforms::extract_Channel(const Image& img, Channel channel) {
    Image out(img.getRows(), img.getColumns(), 1);
    int ch = 0;
    switch (channel) {
        case Channel::RED: ch = 0; break;
        case Channel::GREEN: ch = 1; break;
        case Channel::BLUE: ch = 2; break;
        default: ch = 0;
    }

    for (size_t r = 0; r < img.getRows(); r++)
        for (size_t c = 0; c < img.getColumns(); c++)
            out.at(r, c, 0) = img.at(r,c,ch);

    return out;
}
Image Transforms::to_grayscale(const Image& img){
    if (img.getChannels() == 1){
        std::cout << "Image is already grayscale, nothing to be done.";
        return img;
    }

    Image out(img.getRows(), img.getColumns(), 1);

    for (size_t r = 0; r < img.getRows(); r++)
    {
        for (size_t c = 0; c < img.getColumns(); c++)
        {
            uint8_t R = img.at(r,c,0);
            uint8_t G = img.at(r,c,1);
            uint8_t B = img.at(r,c,2);

            uint8_t gray = static_cast<uint8_t>(
                0.299 * R + 0.587 * G + 0.114 * B
            );

            out.at(r,c,0) = gray;
        }
    }

    return out;
}