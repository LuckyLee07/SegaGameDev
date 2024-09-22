#include "State.h"
#include <iostream>
#include <algorithm>
#include "GameLib/Framework.h"

using namespace GameLib;

State::State(const char* stageData, int dataSize)
{
	setSize(stageData, dataSize);
	m_states.init(m_width, m_height);
	m_goalFlags.init(m_width, m_height);

	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			m_states(x, y) = OBJ_BLOCK;
			m_goalFlags(x, y) = false;
		}
	}
	this->initData(stageData, dataSize);
}

void State::setSize(const char* stageData, int dataSize)
{
	int width = 0;
	int height = 0;

	int index = 0;
	int x = 0, y = 0;
	const char* d = stageData;
	while (index < dataSize)
	{
		Object s;
		switch (d[index])
		{
		case '#':
		case ' ':
		case 'o':
		case 'O':
		case '.':
		case 'p':
		case 'P':
			++x; break;
		case '\n': //到下一行
			++y;
			width = std::max(width, x);
			height = std::max(height, y);
			x = 0;
			break;
		}
		++index;
	}
	//最后一行没换行符
	if (x > 0) height = height + 1;

	m_width = width;
	m_height = height;
}

void State::initData(const char* stageData, int dataSize)
{
	int index = 0;
	int x = 0, y = 0;
	const char* d = stageData;
	while (index < dataSize)
	{
		Object s;
		bool goalFlag = false;
		switch (d[index])
		{
		case '#': s = OBJ_WALL; break;
		case ' ': s = OBJ_SPACE; break;
		case '.': s = OBJ_SPACE; goalFlag = true; break;
		case 'o': s = OBJ_BLOCK; break;
		case 'O': s = OBJ_BLOCK; goalFlag = true; break;
		case 'p': s = OBJ_MAN; break;
		case 'P': s = OBJ_MAN; goalFlag = true; break;
		case '\n': //换行处理
			x = 0; ++y;
			s = OBJ_UNKNOWN;
			break;
		default: s = OBJ_UNKNOWN; break;
		}
		++index;
		if (s != OBJ_UNKNOWN)
		{
			m_states(x, y) = s;
			m_goalFlags(x, y) = goalFlag;
			++x;
		}
	}
}

void State::draw()
{
	const char fonts[] = { ' ', '#', 'o', 'p' };
	const char goalfonts[] = { '.', '#', 'O', 'P' };

	const unsigned colors[] = { 0x000000, 0xffffff, 0xff0000, 0x00ff00 };
	const unsigned goalcolors[] = { 0x0000ff, 0xffffff, 0xff00ff, 0x00ffff };

	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			Object o = m_states(x, y);
			char font;
			unsigned color;
			if (m_goalFlags(x, y))
			{
				font = goalfonts[o]; color = goalcolors[o];
			}
			else
			{
				font = fonts[o]; color = colors[o];
			}
			std::cout << font;
			//drawCell(x, y, color);
		}
		std::cout << std::endl;
	}
}

void State::drawCell(int x, int y, unsigned color)
{
	int windowWidth = Framework::instance().width();
	unsigned* vram = Framework::instance().videoMemory();

	for (int i = 0; i < 16; ++i)
	{
		for (int j = 0; j < 16; ++j)
		{
			vram[(y * 16 + i) * windowWidth + (x * 16 + j)] = color;
		}
	}
}

void State::update(char input)
{
	int dx = 0, dy = 0;
	switch (input)
	{
	case 'a': dx = -1; break;
	case 's': dx = 1; break;
	case 'w': dy = -1; break;
	case 'z': dy = 1; break;
	}

	if (dx == 0 && dy == 0)
		return;

	int x, y; // 查找玩家位置
	bool found = false;
	for (y = 0; y < m_height; ++y)
	{
		for (x = 0; x < m_width; ++x)
		{
			int state = m_states(x, y);
			if (state == OBJ_MAN)
			{
				found = true; break;
			}
		}
		if (found) break;
	}

	// 移动后的位置
	int tx = x + dx;
	int ty = y + dy;
	if (tx < 0 || ty < 0 || tx >= m_width || ty >= m_height)
		return;

	int tps = m_states(tx, ty);
	if (tps == OBJ_SPACE)
	{
		m_states(tx, ty) = OBJ_MAN;
		m_states(x, y) = OBJ_SPACE;
	}
	else if (tps == OBJ_BLOCK)
	{
		// 检测沿该方向的第二个网格位置是否在允许范围内
		int tx2 = tx + dx;
		int ty2 = ty + dy;

		if (tx2 < 0 || ty2 < 0 || tx2 >= m_width || ty2 >= m_height)
			return;

		int tp2s = m_states(tx2, ty2); //沿该方向第二个网格位置
		if (tp2s == OBJ_SPACE)
		{
			m_states(tx2, ty2) = OBJ_BLOCK;
			m_states(tx, ty) = OBJ_MAN;
			m_states(x, y) = OBJ_SPACE;
		}
	}
}

bool State::checkClear()
{
	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			int o = m_states(x, y);
			if (OBJ_BLOCK == o && !m_goalFlags(x, y))
			{
				return false;
			}
		}
	}
	return true;
}
