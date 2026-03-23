#ifndef INTENSITY_H
#define INTESNITY_H

#pragma once
#include "Image.h"

class Intensity{
public:
	static Image::pixel invert(Image::pixel px);
	static Image::pixel log(Image::pixel px, int c);
	static Image::pixel gamma(Image::pixel px, double c);
	static Image::pixel threshold(Image::pixel px, int c, int g);

private:
	Intensity() = delete;
};

#endif