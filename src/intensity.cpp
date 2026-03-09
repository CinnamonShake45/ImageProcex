#include "intensity.h"

Image invert(const Image& img){
    Image invImg(img.getRows(), img.getColumns(), img.getChannels());
	for(size_t r = 0; r < img.getRows(); r++)	{
		for (size_t c = 0; c < img.getColumns(); c++) {
			for (size_t ch = 0; ch < img.getChannels(); ch++) {
				invImg.at(r, c, ch) = 255 - img.at(r, c, ch);
			}
		}
	}
    return invImg;
}