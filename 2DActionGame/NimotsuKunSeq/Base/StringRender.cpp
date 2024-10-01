#include "StringRender.h"
#include "Base/Image.h"
#include "GameLib/Framework.h"

using namespace GameLib;

//静态变量需要cpp中的实体
StringRender* StringRender::m_pInstance = nullptr;

StringRender::StringRender(const char* fontFileName)
{
	m_pFontImage = new Image(fontFileName);
}

StringRender::~StringRender()
{
	SAFE_DELETE(m_pFontImage);
}

StringRender* StringRender::instance()
{
	return m_pInstance;
}

void StringRender::create(const char* fontFileName)
{
	ASSERT(!m_pInstance && "不允许创建两个对象！");
	m_pInstance = new StringRender(fontFileName);
}

void StringRender::destroy()
{
	SAFE_DELETE(m_pInstance);
}

void StringRender::draw(int x, int y, const char* str, unsigned color) const
{
	// 加个容错
	if (str == nullptr) return;
	
	// 设置字体宽/高
	const int CHAR_WIDTH = 8;
	const int CHAR_HEIGHT = 16;

	int dstX = x * CHAR_WIDTH;
	int dstY = y * CHAR_HEIGHT;
	Framework f = Framework::instance();

	// 窗口范围检查
	if (dstY < 0 || dstY >= f.height())
		return;

	for (int i = 0; str[i] != '\0'; ++i)
	{
		int t = str[i];
		// 超出范围按127处理
		if (t < 32 || t >= 128) t = 127;
		t -= 32; // 从32开始
		int srcX = (t % 16) * CHAR_WIDTH;
		int srcY = (t / 16) * CHAR_HEIGHT;
		m_pFontImage->draw(dstX, dstY, srcX, srcY, CHAR_WIDTH, CHAR_HEIGHT);
		dstX += CHAR_WIDTH; //移动到下个字符
	}
}

