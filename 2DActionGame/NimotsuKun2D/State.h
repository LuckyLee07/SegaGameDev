#ifndef __STATE_H__
#define __STATE_H__

#include "Array2D.h"

class Image;

class State
{
public:
	State(const char* stageData, int dataSize);

	void draw() const;
	void update(int dx, int dy, int dt);
	bool hasCleared() const;
	
private:
	class Object; //声明
	void setSize(const char* stageData, int dataSize);
	void initData(const char* stageData, int dataSize);
	
	//void drawCell(int x, int y, unsigned color) const;
	//void drawCell(int x, int y, ImageID imageId) const;
private:
	int m_width;
	int m_height;
	Image* m_image;
	Array2D<Object> m_objects;
	int m_movetCount;
};

#endif  // __STATE_H__