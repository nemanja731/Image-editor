#include "Layer.h"
#include "Pixel.h"
#include <iostream>
using namespace std;

Layer::Layer()
{
	height = width = 0;
	opacity = 100;
	layer = nullptr;
}

Layer::Layer(int height, int width)
{
	layer = new Pixel **[height];
	for (int i = 0; i < height; i++)
		layer[i] = new Pixel *[width];
	for (int i = height - 1; i >= 0; i--)
		for (int j = 0; j < width; j++)
			layer[i][j] = nullptr;
	height = h;
	width = w;
	opacity = 100;
}

Layer::~Layer()
{
	height = width = opacity = 0;
	delete next;
	delete prev;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
			if (layer[i][j] != nullptr)
			{
				delete layer[i][j];
				layer[i][j] = nullptr;
			}
		delete[] layer[i];
	}
	delete[] layer;
}
