#include "Menu.h"
#include "State.h"
#include "Gamebase.h"
#include "Base/Image.h"
#include "Base/StringRender.h"
#include "GameLib/Framework.h"

using namespace GameLib;

Menu::Menu() : m_pImage(nullptr), m_cursorPos(0)
{
	m_pImage = new Image("assets/image/menu.dds");
	m_cursorPos = 1; // 默认选1
}

Menu::~Menu()
{
	SAFE_DELETE(m_pImage);
}

void Menu::update(GameBase* parent)
{
	/*
	const int MAX_STAGE = 4;

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
		switch (m_cursorPos)
		{
		case 1: // 重来
			parent->state()->reset();
			parent->moveTo(GameBase::SEQ_PLAY);
			break;
		case 2: // 关卡
			parent->moveTo(GameBase::SEQ_STATE_SELECT);
			break;
		case 3: // 到主题
			parent->moveTo(GameBase::SEQ_TITLE);
			break;
		case 4: // 继续
			parent->moveTo(GameBase::SEQ_PLAY);
			break;
		default:
			break;
		}
	}
	*/
	int inputNum = 0;
	char numChars[] = { '0', '1', '2', '3', '4' };

	Framework f = Framework::instance();
	for (int index = 0; index < 5; ++index)
	{
		if (f.isKeyTriggered(numChars[index]))
		{
			inputNum = index;
		}
	}
	switch (inputNum)
	{
	case 1: // 重来
		parent->state()->reset();
		parent->moveTo(GameBase::SEQ_PLAY);
		break;
	case 2: // 关卡
		parent->moveTo(GameBase::SEQ_STATE_SELECT);
		break;
	case 3: // 到主题
		parent->moveTo(GameBase::SEQ_TITLE);
		break;
	case 4: // 继续
		parent->moveTo(GameBase::SEQ_PLAY);
		break;
	default:
		break;
	}
	// 先绘制游戏画面
	parent->state()->draw();

	// 在上面再绘制庆祝消息
	m_pImage->draw();

	// 绘制文字
	StringRender* sRender = StringRender::instance();
	sRender->draw(0, 0, "[MENU]");
	sRender->draw(1, 1, "RETRY");
	sRender->draw(1, 2, "GO TO STAGE SELECTION");
	sRender->draw(1, 3, "GO TO TITLE");
	sRender->draw(1, 4, "CONTINUE");
	// 写光标
	sRender->draw(0, m_cursorPos, ">");
}