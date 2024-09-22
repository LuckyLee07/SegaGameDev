#include <iostream>
#include "Image.h"
#include "GameLib/Framework.h"

using namespace GameLib;

void drawPixels()
{
	Framework& instance = Framework::instance();

	unsigned* vram = instance.videoMemory();
	int width = instance.width();
	int height = instance.height();

	static unsigned i;
	vram[i] += i * 100;
	i += 9973; //小于1万的最大的质数
	i %= (width * height);
}

void drawImage(const char* filePath)
{
	static Image* gImage = NULL;
	if (gImage == NULL)
	{
		gImage = new Image(filePath);
	}
	
	int width = 48, height = 48;
	int srcX = gImage->width() * 0.5f - 24;
	int srcY = gImage->height() * 0.5f - 40;

	int dstX = 100, dstY = 100;
	//gImage->draw();
	//gImage->draw(dstX, dstY);

	Framework& instance = Framework::instance();
	int xp = instance.width() / width;
	int yp = instance.height() / height;
	for (int y = 0; y < yp; ++y)
	{
		for (int x = 0; x < xp; ++x)
		{
			gImage->draw(x * width, y * height, srcX, srcY, width, height);
		}
	}
}

namespace GameLib {
	void Framework::update() {
		//drawPixels();
		drawImage("bar.dds");
	}
}