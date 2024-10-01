#include "Title.h"
#include "Parent.h"
#include "Base/Image.h"
#include "Base/StringRender.h"
#include "GameLib/Framework.h"

using namespace GameLib;

Title::Title() : m_pImage(nullptr)
{
	m_pImage = new Image("assets/dummy.dds");
}

Title::~Title()
{
	SAFE_DELETE(m_pImage);
}

void Title::update(Parent* parent)
{
	if (Framework::instance().isKeyTriggered(' '))
	{
		parent->moveTo(Parent::SEQ_STATE_SELECT);
	}
	m_pImage->draw(); //绘制
	// 输出文字
	StringRender::instance()->draw(0, 0, "TITLE: NimotsuKun", 0x00ffff);
	StringRender::instance()->draw(0, 1, "PRESS SPACE KEY", 0xffff00);
}