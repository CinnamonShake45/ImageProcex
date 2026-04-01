#ifndef TRANSFORMS_H
#define TRANSFORMS_H

#include "Image.h"
#include "intensity.h"

enum class Channel {
    RED,
    GREEN,
    BLUE
};

class Transforms {
public:
    template<typename Func>
    static Image apply_transform(const Image& img, Func f) {
        Image out = img;
        for (auto& px : out)
            px = f(px);
        return out;
    }

    static Image extract_Channel(const Image& img, Channel ch);
	static Image to_grayscale(const Image& img);
private:
    Transforms() = delete;
};

#endif