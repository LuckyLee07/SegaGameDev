#include <iostream>
#include "GameLib/Framework.h"

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
const char gStageData[] = "\
########\n\
# .. p #\n\
# oo   #\n\
#      #\n\
########";
const int gStateWidth = 8;
const int gStateHeight = 5;

void initialize(Object* state, int w, int h, const char* stageData);
void draw(const Object* state, int w, int h);
void update(Object* state, char input, int w, int h);
bool checkClear(const Object* state, int w, int h);

void initialize(Object* state, int w, int h, const char* stageData)
{
	int x = 0, y = 0;
	const char* d = stageData;
	while (*d != '\0')
	{
		Object t;
		switch (*d)
		{
		case '#': t = OBJ_WALL; break;
		case ' ': t = OBJ_SPACE; break;
		case 'o': t = OBJ_BLOCK; break;
		case 'O': t = OBJ_BLOCK_ON_GOAL; break;
		case '.': t = OBJ_GOAL; break;
		case 'p': t = OBJ_MAN; break;
		case 'P': t = OBJ_MAN_ON_GOAL; break;
		case '\n': //到下一行
			x = 0;
			++y;
			t = OBJ_UNKNOWN;
			break;
		default: t = OBJ_UNKNOWN; break;
		}
		++d;
		if (t != OBJ_UNKNOWN)
		{
			state[y * w + x] = t;
			++x;
		}
	}
}

void draw(const Object* state, int w, int h)
{
	const char font[] = { ' ', '#', '.', 'o', 'O', 'p', 'P' };
	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			Object o = state[y * w + x];
			std::cout << font[o];
		}
		std::cout << std::endl;
	}
}

void update(Object* state, char input, int w, int h)
{
	int dx = 0, dy = 0;
	switch (input)
	{
	case 'a': dx = -1; break;
	case 's': dx = 1; break;
	case 'w': dy = -1; break;
	case 'z': dy = 1; break;
	}
	// 查找玩家位置
	int i = -1;
	for (i = 0; i < w * h; ++i)
	{
		if (state[i] == OBJ_MAN || state[i] == OBJ_MAN_ON_GOAL)
		{
			break;
		}
	}

	int x = i % w;
	int y = i / w;

	// 移动后的位置
	int tx = x + dx;
	int ty = y + dy;
	if (tx < 0 || ty < 0 || tx >= w || ty >= h) 
		return;

	int p = y * w + x; //玩家位置
	int tp = ty * w + tx; // 目标位置
	if (state[tp] == OBJ_SPACE || state[tp] == OBJ_GOAL)
	{
		state[tp] = state[tp] == OBJ_GOAL ? OBJ_MAN_ON_GOAL : OBJ_MAN;
		state[p] = state[p] == OBJ_MAN_ON_GOAL ? OBJ_GOAL : OBJ_SPACE;
	}
	else if (state[tp] == OBJ_BLOCK || state[tp] == OBJ_BLOCK_ON_GOAL)
	{
		// 检测沿该方向的第二个网格位置是否在允许范围内
		int tx2 = tx + dx;
		int ty2 = ty + dy;

		if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h) 
			return;

		int tp2 = ty2 * w + tx2; //沿该方向第二个网格位置
		if (state[tp2] == OBJ_SPACE || state[tp2] == OBJ_GOAL)
		{
			state[tp2] = state[tp2] == OBJ_GOAL ? OBJ_BLOCK_ON_GOAL : OBJ_BLOCK;
			state[tp] = state[tp] == OBJ_BLOCK_ON_GOAL ? OBJ_MAN_ON_GOAL : OBJ_MAN;
			state[p] = state[p] == OBJ_MAN_ON_GOAL ? OBJ_GOAL : OBJ_SPACE;
		}
	}
}

bool checkClear(const Object* state, int w, int h)
{
	for (int i = 0; i < w * h; ++i)
	{
		if (state[i] == OBJ_BLOCK)
		{
			return false;
		}
	}
	return true;
}

namespace GameLib {
	void Framework::update() {
	}
}

int main()
{
	Object* state = new Object[gStateWidth * gStateHeight];
	initialize(state, gStateWidth, gStateHeight, gStageData);

	while (true)
	{
		draw(state, gStateWidth, gStateHeight);
		if (checkClear(state, gStateWidth, gStateHeight))
		{
			break;
		}
		std::cout << "a:left  s:right w:up z:down. command?" << std::endl;
		char input;
		std::cin >> input;

		update(state, input, gStateWidth, gStateHeight);
	}

	std::cout << "Congraturation's! you win." << std::endl;
	delete[] state;
	state = 0;

	return 0;
}