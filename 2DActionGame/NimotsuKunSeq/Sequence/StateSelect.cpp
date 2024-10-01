#include "StateSelect.h"
#include "Parent.h"
#include "Base/Image.h"
#include "Base/StringRender.h"
#include "GameLib/Framework.h"
#include <sstream>

using namespace GameLib;

StateSelect::StateSelect() : m_pImage(nullptr), m_cursorPos(0)
{
	m_pImage = new Image("assets/dummy.dds");
	m_cursorPos = 1; // 默认选择第一关
}

StateSelect::~StateSelect()
{
	SAFE_DELETE(m_pImage);
}

void StateSelect::update(Parent* parent)
{
	const int MAX_STAGE = 9;
	
	Framework f = Framework::instance();
	if (f.isKeyTriggered('w'))
	{
		--m_cursorPos;
		if (m_cursorPos <= 0)
			m_cursorPos = MAX_STAGE;
	}
	else if (f.isKeyTriggered('s'))
	{
		++m_cursorPos;
		if (m_cursorPos > MAX_STAGE)
			m_cursorPos = 1;
	}
	else if (f.isKeyTriggered(' '))
	{
		parent->setStateId(m_cursorPos);
		parent->moveTo(Parent::SEQ_GAME);
	}

	m_pImage->draw(); //绘制

	// 输出文字
	StringRender* sRenderer = StringRender::instance();
	sRenderer->draw(0, 0, "STAGE SELECTION");

	std::ostringstream oss;
	for (int i = 1; i <= MAX_STAGE; ++i)
	{
		oss << i;
		sRenderer->draw(1, i, oss.str().c_str());
		oss.str("");
	}
	// 写光标
	sRenderer->draw(0, m_cursorPos, ">");
}