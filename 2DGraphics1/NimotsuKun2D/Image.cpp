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
	Framework& instance = Framework::instance();
	unsigned* vram = instance.videoMemory();

	int winWidth = instance.width();
	int winHeight = instance.height();

	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			int pos = (y + dstY) * winWidth + (x + dstX);
			vram[pos] = m_data[y * m_width + x];
		}
	}
}

void Image::draw(int dstX, int dstY, int srcX, int srcY, int w, int h) const
{
	Framework& instance = Framework::instance();
	unsigned* vram = instance.videoMemory();

	int winWidth = instance.width();
	int winHeight = instance.height();
	
	//int dstX = 100, dstY = 100;
	//int srcX = imgWidth * 0.5f - 24;
	//int srcY = imgHeight * 0.5f - 40;

	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			int dstpos = (y + dstY) * winWidth + (x + dstX);
			vram[dstpos] = m_data[(y + srcY) * m_width + (x + srcX)];
		}
	}
}
