#ifndef __STATE_H__
#define __STATE_H__

#include "Base/Array2D.h"

class Image;

class State
{
public:
	State(const char* stageData, int dataSize);
	~State();

	void draw() const;
	void update(int dx, int dy, int dt);
	bool hasCleared() const;

	void reset(); //重置数据
	
private:
	class Object; //声明
	void setSize(const char* stageData, int dataSize);
	void initData(const char* stageData, int dataSize);
	
	//void drawCell(int x, int y, unsigned color) const;
	//void drawCell(int x, int y, ImageID imageId) const;
private:
	int m_width;
	int m_height;
	Image* m_pImage;
	Array2D<Object> m_objects;
	int m_movetCount;

	int m_dataSize;
	char* m_pStageData;
};

#endif  // __STATE_H__