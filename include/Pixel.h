#ifndef PIXEL_H
#define PIXEL_H

class Pixel
{
public:
	Pixel(int b, int g, int r) : b(b), g(g), r(r){};
	Pixel() : b(0), g(0), r(0){};

	int b, g, r;
};

#endif
