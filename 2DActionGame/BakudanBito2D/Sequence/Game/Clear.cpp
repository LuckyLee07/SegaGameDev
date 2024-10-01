#include "Clear.h"
#include "GameBase.h"
#include "State.h"
#include "Base/Image.h"
#include "Base/StringRender.h"
#include "GameLib/Framework.h"

using namespace GameLib;

Clear::Clear() : m_count(0), m_pImage(nullptr)
{
	m_pImage = new Image("assets/image/clear.dds");
}

Clear::~Clear()
{
	SAFE_DELETE(m_pImage);
}

void Clear::update(GameBase* parent)
{
	if (m_count == 60)
	{
		parent->moveTo(GameBase::SEQ_STATE_SELECT);
	}
	// 先绘制游戏画面
	parent->state()->draw();

	// 在上面再绘制庆祝消息
	m_pImage->draw();

	//画清楚和字母
	StringRender::instance()->draw(0, 0, "CLEAR!");

	++m_count;
}