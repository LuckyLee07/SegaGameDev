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

void Image::draw(int dstX, int dstY, int srcX, int srcY, int w, int h) const
{
	Framework& instance = Framework::instance();
	unsigned* vram = instance.videoMemory();

	int winWidth = instance.width();
	int winHeight = instance.height();

	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			int srcpos = (y + srcY) * m_width + (x + srcX);
			int dstpos = (y + dstY) * winWidth + (x + dstX);
			int alpha = (m_data[srcpos] & 0xff000000) >> 24;
			if (alpha >= 128)
				vram[dstpos] = m_data[srcpos];
		}
	}
}
