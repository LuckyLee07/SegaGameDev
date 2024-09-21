#include <iostream>
#include <fstream>
#include <windows.h>
#include <cstdio> // For freopen
#include "GameLib/Framework.h"

using namespace GameLib;

char* readFile(const char* fileName, int& fileSize)
{
	char cwd[MAX_PATH]; //获取当前工作目录
	if (GetCurrentDirectoryA(MAX_PATH, cwd)) {
		std::cout << "Current working directory: " << cwd << std::endl;
	}
	std::ifstream inputFile(fileName, std::ifstream::binary);
	inputFile.seekg(0, std::ifstream::end);
	fileSize = static_cast<int>(inputFile.tellg());
	if (fileSize <= 0) return NULL;

	inputFile.seekg(0, std::ifstream::beg);
	char* fileImage = new char[fileSize];

	if (!inputFile.read(fileImage, fileSize))
	{
		delete[] fileImage;
		return NULL;
	}
	return fileImage;
}

void mainInit()
{
	// 创建控制台
	AllocConsole();

	// 将标准输出重定向到控制台
	FILE* stream;
	freopen_s(&stream, "CONOUT$", "w", stdout);
}

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

unsigned getUnsigned(const char* p)
{
	const unsigned char* up;
	up = reinterpret_cast<const unsigned char*>(p);
	
	unsigned ret = up[0];
	ret |= (up[1] << 8);
	ret |= (up[1] << 16);
	ret |= (up[1] << 24);

	return ret;
}

static char* gFileImage = NULL;
void drawImage(const char* filePath)
{
	static unsigned* image = NULL;
	static unsigned imgHeight, imgWidth;
	if (gFileImage == NULL)
	{
		int fileSize = 0;
		gFileImage = readFile(filePath, fileSize);

		imgHeight = getUnsigned(&gFileImage[12]);
		imgWidth = getUnsigned(&gFileImage[16]);
		std::cout << imgHeight << "," << imgWidth << std::endl;
		//imgHeight = *reinterpret_cast<unsigned*>(&gFileImage[12]);
		//imgWidth = *reinterpret_cast<unsigned*>(&gFileImage[16]);

		image = new unsigned[imgWidth * imgHeight];
		for (unsigned i = 0; i < imgWidth * imgHeight; ++i)
		{
			image[i] = getUnsigned(&gFileImage[128 + i * 4]);
		}
	}
	
	Framework& instance = Framework::instance();
	unsigned* vram = instance.videoMemory();
	int winWidth = instance.width();
	int winHeight = instance.height();

	int width = 48, height = 48;
	int srcX = imgWidth * 0.5f - 24;
	int srcY = imgHeight * 0.5f - 40;

	int dstX = 100, dstY = 100;

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			int dstpos = (y + dstY) * winWidth + (x + dstX);
			vram[dstpos] = image[(y + srcY) * imgWidth + (x + srcX)];
		}
	}
}

static bool gInit = false;

namespace GameLib {
	void Framework::update() {
		if (!gInit)
		{
			gInit = true;
			//mainInit();
		}
		//drawPixels();

		drawImage("bar.dds");
	}
}