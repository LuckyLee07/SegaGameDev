#include "State.h"
#include <iostream>
#include <algorithm>
#include "Base/Image.h"
#include "GameLib/Framework.h"

using namespace GameLib;

const int MAX_MOVE_COUNT = 300; //0.3秒

class State::Object
{
public:
	enum Type
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
		IMAGE_ID_GOAL,
		IMAGE_ID_SPACE,
	};

	Object(Type type= OBJ_BLOCK, bool goal=false);
	void set(char c);
	void move(int dx, int dy, Type repType);
	void drawBackground(int x, int y, const Image* image);
	void drawForeground(int x, int y, const Image* image, int moveCount);
	bool isBlock() const;
	
	Type m_type;
	bool m_goalFlag;
	int m_moveX;
	int m_moveY;
};

State::Object::Object(Type type, bool goal):
	m_type(type), m_goalFlag(goal), m_moveX(0), m_moveY(0)
{
}
void State::Object::set(char c)
{
	switch (c)
	{
	case '#': m_type = OBJ_WALL; break;
	case ' ': m_type = OBJ_SPACE; break;
	case '.': m_type = OBJ_SPACE; m_goalFlag = true; break;
	case 'o': m_type = OBJ_BLOCK; break;
	case 'O': m_type = OBJ_BLOCK; m_goalFlag = true; break;
	case 'p': m_type = OBJ_MAN; break;
	case 'P': m_type = OBJ_MAN; m_goalFlag = true; break;
	}
}
void State::Object::move(int dx, int dy, Type repType)
{
	m_type = repType;
	m_moveX = dx; m_moveY = dy;
}
void State::Object::drawBackground(int x, int y, const Image* image)
{
	ImageID id = IMAGE_ID_SPACE;
	if (m_type == OBJ_WALL)
	{
		id = IMAGE_ID_WALL;
	}
	else if (m_goalFlag)
	{
		id = IMAGE_ID_GOAL;
	}

	int blockw = 32, blockh = 32;
	int posx = x * blockw, posy = y * blockh;
	image->draw(posx, posy, id * blockw, 0, blockw, blockh);
}
void State::Object::drawForeground(int x, int y, const Image* image, int moveCount)
{
	ImageID id = IMAGE_ID_SPACE;
	if (m_type == OBJ_BLOCK)
	{
		id = IMAGE_ID_BLOCK;
	}
	else if (m_type == OBJ_MAN)
	{
		id = IMAGE_ID_PLAYER;
	}
	if (id != IMAGE_ID_SPACE)
	{
		int blockw = 32, blockh = 32; //图片块大小
		int m = MAX_MOVE_COUNT;
		int posx = x * blockw - m_moveX * (m-moveCount) * blockw /m;
		int posy = y * blockh - m_moveY * (m-moveCount) * blockh /m;
		//if (m_moveX != 0 || m_moveY != 0)
		//	cout << "draw ===>>" << posx << ":" << posy << endl;
		image->draw(posx, posy, id * blockw, 0, blockw, blockh);
	}
}
bool State::Object::isBlock() const
{
	return m_type == OBJ_BLOCK && !m_goalFlag;
}


State::State(const char* stageData, int dataSize)
	: m_movetCount(0), m_dataSize(dataSize)
{
	setSize(stageData, dataSize);
	m_objects.init(m_width, m_height);

	m_pStageData = new char[dataSize + 1];
	for (int i = 0; i < dataSize; ++i)
	{
		m_pStageData[i] = stageData[i];
	}
	m_pStageData[dataSize] = '\0'; // NULL终止

	this->initData(stageData, dataSize);

	m_pImage = new Image("assets/nimotsuKunImage2.dds");
}

State::~State()
{
	SAFE_DELETE(m_pImage);
	SAFE_DELETE_ARRAY(m_pStageData);
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
		switch (d[index])
		{
		case '#':
		case ' ': case '.':
		case 'o': case 'O':
		case 'p': case 'P':
			m_objects(x, y).set(d[index]);
			x++; break;
		case '\n': //换行处理
			x = 0; ++y;
			break;
		}
		++index;
	}
}

void State::draw() const
{
	const char fonts[] = { ' ', '#', 'o', 'p', '.', '#', 'O', 'P' };
	// 先绘制背景
	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			Object obj = m_objects(x, y);
			obj.drawBackground(x, y, m_pImage);
		}
	}
	// 其次绘制前景
	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			Object obj = m_objects(x, y);
			obj.drawForeground(x, y, m_pImage, m_movetCount);
		}
	}
}
/*
void State::drawCell(int x, int y, unsigned color) const
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
*/

void State::update(int dx, int dy, int dt)
{
	if (dx == 0 && dy == 0 && m_movetCount == 0)
		return;

	// 移动计数值达到 MAX 后
	if (m_movetCount >= MAX_MOVE_COUNT)
	{
		m_movetCount = 0;
		for (int y = 0; y < m_height; ++y)
		{
			for (int x = 0; x < m_width; ++x)
			{
				m_objects(x, y).m_moveX = 0;
				m_objects(x, y).m_moveY = 0;
			}
		}
	}
	// 移动过程中忽略更新
	if (m_movetCount > 0)
	{
		m_movetCount += dt;
		if (m_movetCount > MAX_MOVE_COUNT)
			m_movetCount = MAX_MOVE_COUNT;
		return;
	}

	int x, y; // 查找玩家位置
	bool found = false;
	for (y = 0; y < m_height; ++y)
	{
		for (x = 0; x < m_width; ++x)
		{
			int state = m_objects(x, y).m_type;
			if (state == Object::OBJ_MAN)
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

	int tps = m_objects(tx, ty).m_type;
	if (tps == Object::OBJ_SPACE)
	{
		//m_objects(tx, ty).m_type = Object::OBJ_MAN;
		//m_objects(x, y).m_type = Object::OBJ_SPACE;
		m_objects(tx, ty).move(dx, dy, Object::OBJ_MAN);
		m_objects(x, y).move(dx, dy, Object::OBJ_SPACE);
		m_movetCount = 1; //移动开始
	}
	else if (tps == Object::OBJ_BLOCK)
	{
		// 检测沿该方向的第二个网格位置是否在允许范围内
		int tx2 = tx + dx;
		int ty2 = ty + dy;

		if (tx2 < 0 || ty2 < 0 || tx2 >= m_width || ty2 >= m_height)
			return;

		int tp2s = m_objects(tx2, ty2).m_type; //沿该方向第二个网格位置
		if (tp2s == Object::OBJ_SPACE)
		{
			//m_objects(tx2, ty2).m_type = Object::OBJ_BLOCK;
			//m_objects(tx, ty).m_type = Object::OBJ_MAN;
			//m_objects(x, y).m_type = Object::OBJ_SPACE;
			m_objects(tx2, ty2).move(dx, dy,Object::OBJ_BLOCK);
			m_objects(tx, ty).move(dx, dy, Object::OBJ_MAN);
			m_objects(x, y).move(dx, dy, Object::OBJ_SPACE);
			m_movetCount = 1; //移动开始
		}
	}
}

bool State::hasCleared() const
{
	if (m_movetCount > 0) return false;
	
	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			const Object& o = m_objects(x, y);
			if (o.isBlock())
			{
				return false;
			}
		}
	}
	return true;
}

void State::reset()
{
	this->initData(m_pStageData, m_dataSize);
}
