#include "ImageEditor.h"

using namespace std;

const int numberOfTests = 12;
// index of test
int testNumber;
// method calls the editImage method which manipulates
// the image depending on the index of the test that was run
void test();

unsigned char *editImage(unsigned char *input)
{

	ImageEditor *imageEditor = new ImageEditor;
	unsigned char *output = nullptr;
	switch (testNumber)
	{
	case 0:
		// MyExample
		break;
	case 1: // check
		// LOAD/SAVE
		imageEditor->loadImage(input);
		output = imageEditor->saveImage();
		break;
	case 2: // check
		// FLIP HORIZONTAL
		imageEditor->loadImage(input);
		imageEditor->flipHorizontal();
		output = imageEditor->saveImage();
		break;
	case 3: // check
		// FLIP VERTICAL
		imageEditor->loadImage(input);
		imageEditor->flipVertical();
		output = imageEditor->saveImage();
		break;
	case 4: // check
		// GRAYSCALE
		imageEditor->loadImage(input);
		imageEditor->toGrayScale();
		output = imageEditor->saveImage();
		break;
	case 5:
		// INVERT COLORS												//check
		imageEditor->loadImage(input);
		imageEditor->invertColors();
		output = imageEditor->saveImage();
		break;
	case 6: // check
		// BLUR WITH SIZE 1
		imageEditor->loadImage(input);
		imageEditor->blur(1);
		output = imageEditor->saveImage();
		break;
	case 7: // check
		// BLUR WITH SIZE 16
		imageEditor->loadImage(input);
		imageEditor->blur(16);
		output = imageEditor->saveImage();
		break;
	case 8: // check
		// CROP CENTER OF IMAGE
		imageEditor->loadImage(input);
		imageEditor->crop(32, 32, 64, 64);
		output = imageEditor->saveImage();
		break;
	case 9: // check
		// NEW LAYER: FILL BLACK RECTANGLE IN THE CENTER OF IMAGE
		imageEditor->loadImage(input);
		imageEditor->addLayer();
		imageEditor->fillRect(32, 32, 64, 64);
		output = imageEditor->saveImage();
		break;
	case 10:
		// NEW LAYER: FILL BLACK RECTANGLE IN THE CENTER OF IMAGE AND DELETE MIDDLE OF IT
		imageEditor->loadImage(input);
		imageEditor->addLayer();
		imageEditor->fillRect(32, 32, 64, 64);
		imageEditor->eraseRect(48, 48, 32, 32);
		output = imageEditor->saveImage();
		break;
	case 11: // check
		// NEW LAYER: FILL BLACK RECTANGLE IN THE CENTER OF IMAGE AND SET OPACITY OF LAYER TO 50%
		imageEditor->loadImage(input);
		imageEditor->addLayer();
		imageEditor->fillRect(32, 32, 64, 64);
		imageEditor->setLayerOpacity(50);
		output = imageEditor->saveImage();
		break;
	case 12: // check
		// NEW LAYER: FILL YELLOW RECTANGLE IN THE CENTER OF IMAGE
		imageEditor->loadImage(input);
		imageEditor->addLayer();
		imageEditor->setActiveColor("#FFFF00");
		imageEditor->fillRect(32, 32, 64, 64);
		output = imageEditor->saveImage();
		break;
	}
	delete imageEditor;
	return output;
}

int main()
{
	// this for loop will run all tests [0 - 12]. Modify the 'if' condition to run
	// only certain tests (eg: if(testNumber == 1 || testNumber == 5))
	for (testNumber = 0; testNumber <= numberOfTests; testNumber++)
		if (testNumber == 0)
			test();

	return 0;
}