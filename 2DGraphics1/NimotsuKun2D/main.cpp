#include <iostream>
#include <fstream>
#include <windows.h>
#undef max //conflict
#include <algorithm>
#include "GameLib/Framework.h"

using namespace GameLib;

void drawCell(int x, int y, unsigned color)
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

char* readFile(const char* fileName, int& fileSize)
{
	char cwd[MAX_PATH]; //获取当前工作目录
	if (GetCurrentDirectoryA(MAX_PATH, cwd)) {
		std::cout << "Current working directory: " << cwd << std::endl;
	}
	std::ifstream inputFile(fileName, std::ifstream::binary);
	inputFile.seekg(0, std::ifstream::end);
	fileSize = static_cast<int>(inputFile.tellg());
	if (fileSize <= 0) return NULL;
	
	inputFile.seekg(0, std::ifstream::beg);
	char* fileImage = new char[fileSize];

	if (!inputFile.read(fileImage, fileSize))
	{
		delete[] fileImage;
		return NULL;
	}
	return fileImage;
}

template<class T>
class Array2D
{
public:
	Array2D() :m_data(NULL),
		m_rows(0), m_cols(0)
	{
	}
	Array2D(int rows, int cols) :
		m_rows(rows), m_cols(cols)
	{
		m_data = new int[rows * cols];
	}
	~Array2D()
	{
		delete[] m_data;
		m_data = NULL;
	}
	void init(int rows, int cols)
	{
		m_rows = rows;
		m_cols = cols;
		m_data = new int[rows * cols];
	}
	int& operator()(int row, int col)
	{
		return m_data[row * m_cols + col];
	}
	const int& operator()(int row, int col) const
	{
		return m_data[row * m_cols + col];
	}
private:
	T* m_data;
	int m_rows; //行
	int m_cols; //列
};

class GameObj
{
public:
	GameObj(const char* stageData, int dataSize);

	void draw();
	void update(char input);
	bool checkClear();

private:
	void init(const char* stageData, int dataSize);

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

GameObj::GameObj(const char* stageData, int dataSize)
{
	init(stageData, dataSize);
	m_states.init(m_width, m_height);

	int index = 0;
	int x = 0, y = 0;
	const char* d = stageData;
	while (index < dataSize)
	{
		Object s;
		switch (d[index])
		{
		case '#': s = OBJ_WALL; break;
		case ' ': s = OBJ_SPACE; break;
		case 'o': s = OBJ_BLOCK; break;
		case 'O': s = OBJ_BLOCK_ON_GOAL; break;
		case '.': s = OBJ_GOAL; break;
		case 'p': s = OBJ_MAN; break;
		case 'P': s = OBJ_MAN_ON_GOAL; break;
		case '\n': //到下一行
			x = 0;
			++y;
			s = OBJ_UNKNOWN;
			break;
		default: s = OBJ_UNKNOWN; break;
		}
		++index;
		if (s != OBJ_UNKNOWN)
		{
			m_states(x, y) = s;
			++x;
		}
	}
}

void GameObj::init(const char* stageData, int dataSize)
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

void GameObj::draw()
{
	unsigned* vram = Framework::instance().videoMemory();
	int windowWidth = Framework::instance().width();

	unsigned font[] = { 0x000000, 0xffffff, 0x0000ff, 0xff0000, 0xff00ff, 0x00ff00, 0x00ffff };
	
	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			unsigned color = font[m_states(x, y)];
			drawCell(x, y, color);
		}
	}
}

void GameObj::update(char input)
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
			if (state == OBJ_MAN || state == OBJ_MAN_ON_GOAL)
			{
				found = true;
				break;
			}
		}
		if (found) break;
	}

	// 移动后的位置
	int tx = x + dx;
	int ty = y + dy;
	if (tx < 0 || ty < 0 || tx >= m_width || ty >= m_height)
		return;

	int ps = m_states(x, y);
	int tps = m_states(tx, ty);
	if (tps == OBJ_SPACE || tps == OBJ_GOAL)
	{
		m_states(tx, ty) = tps == OBJ_GOAL ? OBJ_MAN_ON_GOAL : OBJ_MAN;
		m_states(x, y) = ps == OBJ_MAN_ON_GOAL ? OBJ_GOAL : OBJ_SPACE;
	}
	else if (tps == OBJ_BLOCK || tps == OBJ_BLOCK_ON_GOAL)
	{
		// 检测沿该方向的第二个网格位置是否在允许范围内
		int tx2 = tx + dx;
		int ty2 = ty + dy;

		if (tx2 < 0 || ty2 < 0 || tx2 >= m_width || ty2 >= m_height)
			return;

		int tp2s = m_states(tx2, ty2); //沿该方向第二个网格位置
		if (tp2s == OBJ_SPACE || tp2s == OBJ_GOAL)
		{
			m_states(tx2, ty2) = tp2s == OBJ_GOAL ? OBJ_BLOCK_ON_GOAL : OBJ_BLOCK;
			m_states(tx, ty) = tps == OBJ_BLOCK_ON_GOAL ? OBJ_MAN_ON_GOAL : OBJ_MAN;
			m_states(x, y) = ps == OBJ_MAN_ON_GOAL ? OBJ_GOAL : OBJ_SPACE;
		}
	}
}

bool GameObj::checkClear()
{
	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			int o = m_states(x, y);
			if (OBJ_BLOCK == o)
			{
				return false;
			}
		}
	}
	return true;
}

void mainLoop();
GameObj* gState = NULL;

namespace GameLib {
	void Framework::update() {
		mainLoop();
	}
}

void mainLoop()
{
	if (gState == NULL)
	{
		int filesize = 0;
		const char* filepath = "stateData.txt";
		const char* buffer = readFile(filepath, filesize);

		if (buffer == NULL) return;
		
		gState = new GameObj(buffer, filesize);
		delete[] buffer;
		buffer = NULL;

		gState->draw();
	}

	// 主循环通关检测
	bool needClear = false;
	if (gState->checkClear())
		needClear = true;

	// 获取输入并刷新
	std::cout << "a:left  s:right w:up z:down. command?" << std::endl;
	char input;
	std::cin >> input;
	gState->update(input);

	// 绘制
	gState->draw();

	if (needClear)
	{
		std::cout << "Congraturation's! you win." << std::endl;
		delete gState;
		gState = NULL;
	}
}
