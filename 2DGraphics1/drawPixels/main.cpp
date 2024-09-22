#include <iostream>
#include <sstream>
#include <windows.h>
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

void drawImageBlendTest()
{
	static Image* gImage1 = NULL;
	static Image* gImage2 = NULL;
	if (gImage1 == NULL || gImage2 == NULL)
	{
		gImage1 = new Image("assets/forground.dds");
		gImage2 = new Image("assets/background.dds");
	}
	unsigned* vram = Framework::instance().videoMemory();
	int ww = Framework::instance().width();
	int wh = Framework::instance().height();
	for (int i = 0; i < ww * wh; ++i) {
		vram[i] = 0; //用黑色填充屏幕
	}
	gImage2->draw();
	gImage1->draw();
}

// 自定义缓冲区，将输出重定向到 Visual Studio 的调试输出窗口
class DebugStreamBuffer : public std::streambuf {
protected:
	virtual int overflow(int c = EOF) override {
		if (c != EOF) {
			char buffer[] = { static_cast<char>(c), '\0' };
			OutputDebugStringA(buffer);  // 将字符输出到 VS 调试窗口
		}
		return c;
	}
};

static bool g_Init = false;
namespace GameLib {
	void Framework::update() {
		// 创建一个 DebugStreamBuffer 实例并将其绑定到 std::cout
		DebugStreamBuffer debugBuffer;
		std::cout.rdbuf(&debugBuffer);  // 将 std::cout 重定向到自定义缓冲区

		//drawPixels();
		//drawImage("bar.dds");
		drawImageBlendTest();
	}
}