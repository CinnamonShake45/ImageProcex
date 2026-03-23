#include "intensity.h"
#include <cmath>
#include <algorithm>

static Image::pixel clamp_int(int v)
{
	return static_cast<Image::pixel>(std::clamp(v, 0, 255));
}

Image::pixel Intensity::invert(Image::pixel px) {
	Image::pixel result = 255 - px;
	return result;
}

Image::pixel Intensity::log(Image::pixel px, int c) {
	double result = c * std::log(1.0 + px);
	return clamp_int(static_cast<Image::pixel>(result));
}

Image::pixel Intensity::gamma(Image::pixel px, double c) {
	double normalized = px / 255.0;
	double corrected = std::pow(normalized, c);
	return static_cast<Image::pixel>(std::clamp(corrected * 255.0, 0.0, 255.0));
}

Image::pixel Intensity::threshold(Image::pixel px, int c, int g) {
	Image::pixel result = (px >= c) ? clamp_int(g) : 0;
	return result;
}
