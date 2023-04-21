#ifndef IMAGE_EDITOR_H
#define IMAGE_EDITOR_H

#include "Pixel.h"
#include "Layer.h"
#include <string>

using namespace std;

class ImageEditor
{
public:
	ImageEditor();
	~ImageEditor();

	bool loadImage(unsigned char *image);
	unsigned char *saveImage();

	void addLayer();
	void deleteLayer();
	void selectLayer(int i);
	void setLayerOpacity(int);
	void invertColors();
	void toGrayScale();
	void blur(int size);
	void flipHorizontal();
	void flipVertical();
	void crop(int x, int y, int w, int h);
	void setActiveColor(string hex);
	void fillRect(int x, int y, int w, int h);
	void eraseRect(int x, int y, int w, int h`);
	void update();
	void updateFirstLayer();

	int readName(unsigned char *image);
	int readSize(unsigned char *image, int curr);
	bool readPixels(unsigned char *image, int curr);

	bool allocate();
	int findSize();
	void makeFirstLayer();

private:
	int height, width, layersNumber;
	string name;
	Pixel activeColor, **allPixels;
	Layer *currentLayer, *list;
};

#endif