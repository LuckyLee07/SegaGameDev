#include "Loading.h"
#include "GameBase.h"
#include "Base/Image.h"
#include "Base/StringRender.h"
#include "GameLib/Framework.h"

using namespace GameLib;

Loading::Loading() : m_count(0), m_started(false), m_pImage(nullptr)
{
	m_pImage = new Image("assets/image/loading.dds");
}

Loading::~Loading()
{
	SAFE_DELETE(m_pImage);
}

void Loading::update(GameBase* parent)
{
	if (!m_started)
	{
		parent->startLoding();
		m_started = true;
	}
	else if (m_count == 60) //等待1秒
	{
		parent->moveTo(GameBase::SEQ_PLAY);
	}
	
	m_pImage->draw(); //绘制

	// 输出文字
	StringRender::instance()->draw(0, 0, "NOW LOADING...");

	++m_count;
}