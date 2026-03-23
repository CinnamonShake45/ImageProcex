#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H

#include <iostream>
#include "Image.h"

namespace imgutils
{

// Print only basic image information
inline void print_info(const Image& img)
{
    std::cout << "Image Info:\n";
    std::cout << "Height   : " << img.getRows() << "\n";
    std::cout << "Width    : " << img.getColumns() << "\n";
    std::cout << "Channels : " << img.getChannels() << "\n";
}

// Dump full image pixel data to console
inline void print_data(const Image& img)
{
    size_t rows = img.getRows();
    size_t cols = img.getColumns();
    size_t chs  = img.getChannels();

    std::cout << "\nImage Data:\n";

    for (size_t r = 0; r < rows; r++)
    {
        for (size_t c = 0; c < cols; c++)
        {
            std::cout << "(";

            for (size_t ch = 0; ch < chs; ch++)
            {
                std::cout << static_cast<int>(img.at(r, c, ch));

                if (ch != chs - 1)
                    std::cout << ",";
            }

            std::cout << ") ";
        }

        std::cout << "\n";
    }
}

// Print a small region 
inline void print_region(const Image& img,
                         size_t r0, size_t c0,
                         size_t r1, size_t c1)
{
    size_t chs = img.getChannels();

    std::cout << "\nImage Region:\n";

    for (size_t r = r0; r <= r1; r++)
    {
        for (size_t c = c0; c <= c1; c++)
        {
            std::cout << "(";

            for (size_t ch = 0; ch < chs; ch++)
            {
                std::cout << static_cast<int>(img.at(r, c, ch));

                if (ch != chs - 1)
                    std::cout << ",";
            }

            std::cout << ") ";
        }

        std::cout << "\n";
    }
}

} // namespace imgutils

#endif