#ifndef LAYER_H
#define LAYER_H

#include "Pixel.h"

class Layer
{
public:
	Layer();
	Layer(int h, int w);
	~Layer();

	int height, width, opacity;
	Pixel **(*layer);
	Layer *prev, *next;
};

#endif