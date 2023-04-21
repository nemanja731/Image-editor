# Image-editor

## About project

The goal of the project is to implement various image manipulations. The image is represented by a matrix of pixels, and each pixel contains a color determined by an RGB value:

- ![#ff0000](https://placehold.co/15x15/f03c15/f03c15.png) `red (0 - 255)`
- ![#00ff00](https://placehold.co/15x15/c5f015/c5f015.png) `green (0 - 255)`
- ![#0000ff](https://placehold.co/15x15/1589F0/1589F0.png) `blue (0 - 255)`

The image editor has the ability to add new layers (Layer) to the image. When loading an image, a new one is created
layer whose pixel matrix contains the pixels of the loaded image. The newly created layer becomes the active layer. Each layer has an opacity percentage whose value is a non-negative integer with a maximum
with a value of 100. When saving an image, the pixels in the upper layers are drawn over the pixels in the lower
layers.

![ImageEditor](https://i.gifer.com/K5tw.gif)

## Functionality

Moguce manipulacije nad slikom su:
- `addLayer()` -> adds a new image layer above of the active layer and that layer becomes the active layer
- `deleteLayer()` -> deletes the active image layer
- `selectLayer(int i)` -> the i-th layer above the layer with the loaded image becomes the active layer
- `setLayerOpacity(int)` -> changes the percentage opacity of the active layer
- `invertColors()` -> inverts the colors of all pixels of the active layer
- `toGrayScale()` -> and converts the colors of all pixels of the active layer in shades of gray
- `blur(int size)` -> blurs all pixels of the active layer by replacing each pixel with a pixel whose color intensities are obtained as mean values
of the color intensity of the pixel being changed and of all pixels around the pixel being changed to distance from the maximum 'size' rows and 'size' columns
- `flipHorizontal()` -> rotates the pixels along the y axis.
- `flipVertical()` -> rotates the pixels along the x axis.
- `crop(int x, int y, int w, int h)` -> discards all pixels of all layers that are not in the part of the image of width w and height h whose upper left corner is in x and y coordinates
- `setActiveColor(string hex)` -> sets the active color to draw to the color specified through the hex parameter
- `fillRect(int x, int y, int w, int h)` -> draws a rectangle of the active color of width w and height h whose upper left corner is in x and y coordinates
- `eraseRect(int x, int y, int w, int h)` -> removes pixels that are inside a rectangle of width w and height h whose upper left corner is in coordinates x and y

Before each manipulation, it is necessary to load the image with the method `loadImage(unsigned char *image)`, and after finishing the manipulation, it is necessary to save the image with the method `saveImage()`
