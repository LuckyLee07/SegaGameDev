#ifndef __IMAGE_H__
#define __IMAGE_H__

class Image
{
public:
	Image(const char* fileName);
	~Image();
	int width() const;
	int height() const;
	const unsigned* data() const;
	void draw(int dstX = 0, int dstY = 0) const;
	void draw(int dstX, int dstY, int srcX, int srcY, int w, int d, unsigned color = 0) const;
private:
	int m_width;
	int m_height;
	unsigned* m_data;
};

#endif  // __IMAGE_H__