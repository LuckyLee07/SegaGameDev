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
		OBJ_GOAL,
		OBJ_BLOCK,
		OBJ_BLOCK_ON_GOAL,
		OBJ_MAN,
		OBJ_MAN_ON_GOAL,

		OBJ_UNKNOWN,
	};

	int m_width;
	int m_height;
	Array2D<int> m_states;
};

#endif  // __STATE_H__