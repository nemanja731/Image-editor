#include "ImageEditor.h"
#include <iostream>
using namespace std;

ImageEditor::ImageEditor()
{
	int height = width = 0;
	name = "";
	allPixels = nullptr, list = nullptr, currentLayer = nullptr;
	activeColor = Pixel(0, 0, 0);
}

ImageEditor::~ImageEditor()
{
	for (int i = 0; i < height; i++)
		delete[] allPixels[i];
	delete[] allPixels;
	while (list)
	{
		Layer *temporary = list;
		list = list->next;
		delete temporary;
	}
	currentLayer = nullptr;
	name = "";
	height = width = 0;
}

int ImageEditor::readName(unsigned char *image)
{
	int index = 2;
	if (image[index] == '=')
	{
		for (int i = index + 1; image[i] != '='; i++)
		{
			index = i;
			name += static_cast<char>(image[i]);
		}
		index += 2;
	}
	return index;
}

int ImageEditor::readSize(unsigned char *image, int index)
{
	int step;
	step = 1;
	for (int i = 0; i < 4; i++, step *= 256)
	{
		int number = step * image[index++];
		width += number;
		height += number;
	}
	return index;
}

bool ImageEditor::readPixels(unsigned char *image, int index)
{
	try
	{
		for (int i = height - 1; i >= 0; i--)
		{
			for (int j = 0; j < width; j++)
			{
				Pixel temporary = Pixel(image[index], image[index + 1], image[index + 2]);
				allPixels[i][j] = temporary;
				index += 3;
			}
			while (!(index % 4 == 0))
				index++;
		}
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool ImageEditor::allocate()
{
	try
	{
		allPixels = new Pixel *[height];
		for (int i = 0; i < height; i++)
			allPixels[i] = new Pixel[width];
		return true;
	}
	catch (...)
	{
		return false;
	}
}

int ImageEditor::findSize()
{
	int rowSize = width * 3;
	int imageSize;
	while (!(rowSize % 4 == 0))
		rowSize++;
	if (name != "")
	{
		int prefixSize = 3;
		prefixSize += name.size() + 1;
		while (!(prefixSize % 4 == 0))
			prefixSize++;
		prefixSize += 8;
		imageSize = rowSize * height + prefixSize;
	}
	else
		imageSize = (rowSize * height + 12);
	return imageSize;
}

void ImageEditor::makeFirstLayer()
{
	Layer *newLayer = new Layer(height, width);
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			newLayer->layer[i][j] = new Pixel(allPixels[i][j].b, allPixels[i][j].g, allPixels[i][j].r);
	list = newLayer;
	currentLayer = newLayer;
	layersNumber++;
}

bool ImageEditor::loadImage(unsigned char *image)
{
	if (allPixels != NULL)
		this->~ImageEditor();
	int index = readName(image);
	while (!(index % 4 == 0))
		index++;
	index = readSize(image, index);
	int allPixelsAllocated = allocate();
	if (!allPixelsAllocated)
	{
		cout << "\nAllocation failure!\n";
		return false;
	}
	bool pixelRead = readPixels(image, index);
	if (!pixelRead)
	{
		cout << "Reading error!";
		return false;
	}
	makeFirstLayer();
	return true;
}

unsigned char *ImageEditor::saveImage()
{
	update();
	int imageSize = findSize();
	unsigned char *image = new unsigned char[imageSize];
	image[0] = 'B', image[1] = 'M';
	int index = 2, temporary, step = 256;
	if (name != "")
	{
		image[index++] = '=';
		for (int i = 0; i < name.size(); i++)
			image[index++] = name[i];
		image[index++] = '=';
	}
	save(index, height, width, step, temporary);
	return image;
}

void save(int index, int height, int width, int step, int temporary)
{
	while (!(index % 4 == 0))
		index++;
	temporary = width;
	for (int i = 0; i < 4; i++, temporary /= step)
		image[index++] = temporary % step;
	temporary = height;
	for (int i = 0; i < 4; i++, temporary /= step)
		image[index++] = temporary % step;
	for (int i = height - 1; i >= 0; i--)
	{
		for (int j = 0; j < width; j++)
		{
			image[index++] = (allPixels[i][j]).r;
			image[index++] = (allPixels[i][j]).g;
			image[index++] = (allPixels[i][j]).b;
		}
		while (!(index % 4 == 0))
			index++;
	}
}

void ImageEditor::addLayer()
{
	Layer *newLayer = new Layer(height, width);
	if (currentLayer->next == nullptr)
	{
		currentLayer->next = newLayer;
		newLayer->prev = currentLayer;
	}
	else
	{
		newLayer->next = currentLayer->next;
		(currentLayer->next)->prev = newLayer;
		newLayer->prev = currentLayer;
		currentLayer->next = newLayer;
	}
	currentLayer = newLayer;
	layersNumber++;
}

void ImageEditor::deleteLayer()
{
	if (currentLayer->next == nullptr)
	{
		Layer *temporary = currentLayer->prev;
		Layer *old = currentLayer;
		currentLayer = temporary;
		delete old;
		temporary->next = nullptr;
	}
	else
	{
		Layer *temporary = currentLayer->prev;
		(currentLayer->next)->prev = temporary;
		temporary->next = currentLayer->next;
		Layer *old = currentLayer;
		currentLayer = temporary;
		delete old;
	}

	layersNumber--;
}

void ImageEditor::selectLayer(int i)
{
	if (i < layersNumber && i >= 0)
	{
		Layer *temporary = list;
		for (int j = 1; j <= i; j++)
			temporary = temporary->next;
		currentLayer = temporary;
	}
}

void ImageEditor::setLayerOpacity(int op)
{
	currentLayer->opacity = op;
}

void ImageEditor::invertColors()
{
	for (int i = height - 1; i >= 0; i--)
		for (int j = 0; j < width; j++)
			if (currentLayer->layer[i][j])
			{
				(*currentLayer->layer[i][j]).r = 255 - (*currentLayer->layer[i][j]).r;
				(*currentLayer->layer[i][j]).g = 255 - (*currentLayer->layer[i][j]).g;
				(*currentLayer->layer[i][j]).b = 255 - (*currentLayer->layer[i][j]).b;
			}
}

void ImageEditor::toGrayScale()
{
	for (int i = height - 1; i >= 0; i--)
		for (int j = 0; j < width; j++)
			if (currentLayer->layer[i][j])
			{
				changeToGray()
			}
}

void changeToGray()
{
	int red = (*currentLayer->layer[i][j]).r;
	int green = (*currentLayer->layer[i][j]).g;
	int blue = (*currentLayer->layer[i][j]).b;
	int gray = (0.3 * red + 0.59 * green + 0.11 * blue);
	(*currentLayer->layer[i][j]).r = gray;
	(*currentLayer->layer[i][j]).g = gray;
	(*currentLayer->layer[i][j]).b = gray;
}

void ImageEditor::blur(int size)
{
	Pixel **(*temporaryMatrix);
	temporaryMatrix = new Pixel **[height];
	for (int i = 0; i < height; i++)
		temporaryMatrix[i] = new Pixel *[width];
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			blurPixels(size, &temporaryMatrix);
	Layer *current = currentLayer;

	transferPixels(temporaryMatrix);
	deleteTemporaryMatrix(temporaryMatrix);
}

void blurPixels(int size, Pixel ****temporaryMatrix)
{
	int sumR = 0, sumG = 0, sumB = 0, counter = 0;
	for (int k = -size; k <= size; k++)
		for (int p = -size; p <= size; p++)
		{
			int x = i + k;
			int y = j + p;
			if ((x >= 0) && (x < height) && (y >= 0) && (y < width) && currentLayer->layer[x][y])
			{
				sumB += currentLayer->layer[x][y]->b;
				sumG += currentLayer->layer[x][y]->g;
				sumR += currentLayer->layer[x][y]->r;
				counter++;
			}
		}
	if (counter > 0)
	{
		int d = counter;
		sumB = (int)(sumB / d);
		sumG = (int)(sumG / d);
		sumR = (int)(sumR / d);
		temporaryMatrix[i][j] = new Pixel((unsigned char)sumB, (unsigned char)sumG, (unsigned char)sumR);
	}
	else
		temporaryMatrix[i][j] = nullptr;
}

void transferPixels(Pixel ***temporaryMatrix)
{
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			if (current->layer[i][j])
			{
				(*current->layer[i][j]).b = (*temporaryMatrix[i][j]).b;
				(*current->layer[i][j]).r = (*temporaryMatrix[i][j]).r;
				(*current->layer[i][j]).g = (*temporaryMatrix[i][j]).g;
			}
}

void deleteTemporaryMatrix(Pixel ***temporaryMatrix)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
			if (temporaryMatrix[i][j])
			{
				delete temporaryMatrix[i][j];
				temporaryMatrix[i][j] = nullptr;
			}
		delete[] temporaryMatrix[i];
	}
	delete[] temporaryMatrix;
}

void ImageEditor::flipVertical()
{
	for (int i = 0; i < height / 2; i++)
		for (int j = 0; j < width; j++)
		{
			Pixel *temporary = currentLayer->layer[i][j];
			currentLayer->layer[i][j] = currentLayer->layer[height - 1 - i][j];
			currentLayer->layer[height - 1 - i][j] = temporary;
		}
}

void ImageEditor::flipHorizontal()
{
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width / 2; j++)
		{
			Pixel *temporary = currentLayer->layer[i][j];
			currentLayer->layer[i][j] = currentLayer->layer[i][width - 1 - j];
			currentLayer->layer[i][width - 1 - j] = temporary;
		}
}

void ImageEditor::crop(int x, int y, int w, int h)
{
	int left, right, up, down;
	Layer *current = list;
	left = (x >= 0) ? x : 0;
	right = (x + w < width) ? x + w : width;
	int newWidth = right - left;
	up = (y >= 0) ? y : 0;
	down = (y + h < height) ? y + h : height;
	int newHeight = down - up;
	while (current)
	{
		Pixel **(*temporaryMatrix);
		temporaryMatrix = new Pixel **[newHeight];
		cropAndStore(&temporaryMatrix, newHeight, newWidth, up, down, left, right);
		deleteLayers(height, width, &current->layer);
		paste(temporaryMatrix, newHeight, newWidth);
		deleteLayers(newHeight, newWidth, temporaryMatrix);
		height = newHeight;
		width = newWidth;
		current = current->next;
	}
	height = newHeight;
	width = newWidth;
	for (int i = 0; i < height; i++)
		delete[] allPixels[i];
	delete[] allPixels;
	allocate();
}

void paste(Pixel ***temporaryMatrix, int newHeight, int newWidth)
{
	current->layer = new Pixel **[newHeight];
	for (int i = 0; i < newHeight; i++)
		current->layer[i] = new Pixel *[newWidth];
	for (int i = 0; i < newHeight; i++)
		for (int j = 0; j < newWidth; j++)
			current->layer[i][j] = nullptr;
	for (int i = 0; i < newHeight; i++)
		for (int j = 0; j < newWidth; j++)
			if (temporaryMatrix[i][j])
			{
				int red = (*temporaryMatrix[i][j]).r;
				int green = (*temporaryMatrix[i][j]).g;
				int blue = (*temporaryMatrix[i][j]).b;
				current->layer[i][j] = new Pixel(blue, green, red);
			}
			else
				current->layer[i][j] = nullptr;
}

void cropAndStore(Pixel ****temporaryMatrix, int newHeight, int newWidth, int up, int down, int left, int right)
{
	for (int i = 0; i < newHeight; i++)
		*temporaryMatrix[i] = new Pixel *[newWidth];
	for (int i = 0; i < newHeight; i++)
		for (int j = 0; j < newWidth; j++)
			*temporaryMatrix[i][j] = nullptr;
	for (int i = up; i < down; i++)
	{
		for (int j = left; j < right; j++)
			if (current->layer[i][j])
			{
				int red = (*current->layer[i][j]).r;
				int green = (*current->layer[i][j]).g;
				int blue = (*current->layer[i][j]).b;
				*temporaryMatrix[i - up][j - left] = current->layer[i][j];
				current->layer[i][j] = nullptr;
			}
			else
				*temporaryMatrix[i - up][j - left] = nullptr;
	}
}

void deleteLayers(int height, int width, Pixel ****layer)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
			if (*layer[i][j])
			{
				delete *layer[i][j];
				*layer[i][j] = nullptr;
			}
		delete[] * layer[i];
	}
	delete[] * layer;
}

void ImageEditor::setActiveColor(string hex)
{
	string red = hex.substr(1, 2);
	string green = hex.substr(3, 2);
	string blue = hex.substr(5, 2);
	activeColor.r = stoi(red, 0, 16);
	activeColor.g = stoi(green, 0, 16);
	activeColor.b = stoi(blue, 0, 16);
}

void ImageEditor::fillRect(int x, int y, int w, int h)
{
	int left, right;
	int up, down;
	left = (x >= 0) ? x : 0;
	right = (x + w < width) ? x + w : width;
	up = (y >= 0) ? y : 0;
	down = (y + h < height) ? y + h : height;
	Pixel *currentEditor = new Pixel();
	(*currentEditor).r = activeColor.r;
	(*currentEditor).g = activeColor.g;
	(*currentEditor).b = activeColor.b;
	for (int i = up; i < down; i++)
		for (int j = left; j < right; j++)
			if (currentLayer->layer[i][j] == nullptr)
				currentLayer->layer[i][j] = new Pixel((*currentEditor).b, (*currentEditor).g, (*currentEditor).r);
			else
			{
				currentLayer->layer[i][j]->r = (*currentEditor).r;
				currentLayer->layer[i][j]->g = (*currentEditor).g;
				currentLayer->layer[i][j]->b = (*currentEditor).b;
			}
	delete currentEditor;
}

void ImageEditor::eraseRect(int x, int y, int w, int h)
{
	if (currentLayer != list)
	{
		int left, right;
		int up, down;
		left = (x >= 0) ? x : 0;
		right = (x + w < width) ? x + w : width;
		up = (y >= 0) ? y : 0;
		down = (y + h < height) ? y + h : height;
		for (int i = up; i < down; i++)
			for (int j = left; j < right; j++)
			{
				if (currentLayer->layer[i][j])
				{
					delete currentLayer->layer[i][j];
					currentLayer->layer[i][j] = NULL;
				}
			}
	}
}

void ImageEditor::updateFirstLayer()
{
	for (int i = height - 1; i >= 0; i--)
		for (int j = 0; j < width; j++)
			list->layer[i][j] = &allPixels[i][j];
}

void ImageEditor::update()
{
	Layer *last = list;
	for (int i = 1; i < layersNumber; i++)
		last = last->next;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			Layer *current = last;
			double remaining = 1.0;
			int blue = 0, green = 0, red = 0;
			while (current)
			{
				if (current->layer[i][j])
				{
					red += (remaining * current->opacity * current->layer[i][j]->r) * 0.01;
					green += (remaining * current->opacity * current->layer[i][j]->g) * 0.01;
					blue += (remaining * current->opacity * current->layer[i][j]->b) * 0.01;
					remaining *= (100 - current->opacity) * 0.01;
				}
				current = current->prev;
			}
			allPixels[i][j].r = red;
			allPixels[i][j].g = green;
			allPixels[i][j].b = blue;
		}
	}
}
