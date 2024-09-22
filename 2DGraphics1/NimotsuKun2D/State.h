#ifndef __STATE_H__
#define __STATE_H__

#include "Array2D.h"

class State
{
public:
	State(const char* stageData, int dataSize);

	void draw();
	void drawCell(int x, int y, unsigned color);
	void update(char input);
	bool checkClear();

private:
	void setSize(const char* stageData, int dataSize);
	void initData(const char* stageData, int dataSize);

private:
	enum Object
	{
		OBJ_SPACE,
		OBJ_WALL,
		OBJ_BLOCK,
		OBJ_MAN,

		OBJ_UNKNOWN,
	};
	// 网格绘制ID
	enum ImageID
	{
		IMAGE_ID_PLAYER,
		IMAGE_ID_WALL,
		IMAGE_ID_BLOCK,
		IMAGE_ID_BLOCK_ON_GOAL,
		IMAGE_ID_GOAL,
		IMAGE_ID_SPACE,
	};

	int m_width;
	int m_height;
	Array2D<Object> m_states;
	Array2D<bool> m_goalFlags;
};

#endif  // __STATE_H__