#include "Image.h"
#include "File.h"
#include <cstddef> // NULL
#include "GameLib/Framework.h"

using namespace GameLib;

Image::Image(const char* fileName)
{
	File file(fileName);
	m_height = file.getUnsigned(12);
	m_width = file.getUnsigned(16);

	m_data = new unsigned[m_width * m_height];
	for (int i = 0; i < m_width*m_height; ++i)
	{
		m_data[i] = file.getUnsigned(128 + i * 4);
	}
}

Image::~Image()
{
	delete[] m_data;
	m_data = NULL;
}

int Image::width() const
{
	return m_width;
}

int Image::height() const
{
	return m_height;
}

const unsigned* Image::data() const
{
	return m_data;
}

void Image::draw(int dstX, int dstY) const
{
	int srcX = 0, srcY = 0;
	draw(dstX, dstY, srcX, srcY, m_width, m_height);
}

void Image::draw(int dstX, int dstY, int srcX, int srcY, int w, int h, unsigned color) const
{
	Framework& instance = Framework::instance();
	unsigned* vram = instance.videoMemory();

	int winWidth = instance.width();
	int winHeight = instance.height();

	// 逻辑优化 float->unsigned
	int srcpos = srcY * m_width + srcX;
	int dstpos = dstY * winWidth + dstX;
	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			unsigned scolor = m_data[srcpos + x];
			unsigned* dcolor = &vram[dstpos + x];

			unsigned srcA = (scolor & 0xff000000) >> 24;
			unsigned invAlpha = 255 - srcA; // 1 - alpha 的整数形式

			if (color != 0) scolor = color; //如果传了值 则用传进来的
			unsigned srcR = (scolor & 0x00ff0000) >> 16;
			unsigned srcG = (scolor & 0x0000ff00) >> 8;
			unsigned srcB = (scolor & 0x000000ff) >> 0;

			unsigned dstR = (*dcolor & 0xff0000) >> 16;
			unsigned dstG = (*dcolor & 0x00ff00) >> 8;
			unsigned dstB = (*dcolor & 0x0000ff) >> 0;

			//右移一位(>>1)相当于除以2; 右移8位(>>8)相当于除以256 
			unsigned newR = (srcR * srcA + invAlpha * dstR) >> 8;
			unsigned newG = (srcG * srcA + invAlpha * dstG) >> 8;
			unsigned newB = (srcB * srcA + invAlpha * dstB) >> 8;

			*dcolor = (newR << 16) | (newG << 8) | newB;
		}
		srcpos += m_width;
		dstpos += winWidth;
	}
}
