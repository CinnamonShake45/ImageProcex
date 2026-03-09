#include <iostream>
#include "Image.h"
#include "imageread.h"
#include "intensity.h"
#include "imagewrite.h"

void printImageInfo(const Image& img) {
    std::cout << "Image information:\n\n";
    std::cout << "Image height: " << img.getRows() << std::endl;
    std::cout << "Image width: " << img.getColumns() << std::endl;
    std::cout << "Image channel width: " << img.getChannels() << std::endl;
}

void dumpImageData(const Image& img) {
    std::cout << "Dumping image data:\n\n";
    for (size_t r = 0; r < img.getRows(); r++) {
        for (size_t c = 0; c < img.getColumns(); c++) {
            for (size_t ch = 0; ch < img.getChannels(); ch++) {
                std::cout << static_cast<int>(img.at(r, c, ch));

                if (ch != img.getChannels() - 1)
                    std::cout << " ";
            }

            std::cout << "\t";
        }
        std::cout << "\n";
    }
}

int main(){
    const char* imgPath = "../test_images/lena_rgb.png";

    std::cout << "Image path input: " << imgPath << std::endl;
    std::string imgType = ImageReader::get_image_type(imgPath);
    std::cout << "Image type confirmation: " << imgType << std::endl;

    Image img = ImageReader::read_png(imgPath, 1);

    printImageInfo(img);

    // dumpImageData(img);

    Image invImg = invert(img);

    std::cout << "Testing invert() function:\n";

    // dumpImageData(invImg);

    const char* outImgPath = "./out.png";

    ImageWriter::write_png(outImgPath, invImg);

    return 0;
}