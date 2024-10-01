#include "Clear.h"
#include "GameBase.h"
#include "State.h"
#include "Base/Image.h"
#include "GameLib/Framework.h"

using namespace GameLib;

Clear::Clear() : m_count(0), m_pImage(nullptr)
{
	m_pImage = new Image("assets/clear.dds");
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
	// �Ȼ�����Ϸ����
	parent->state()->draw();

	// �������ٻ�����ף��Ϣ
	m_pImage->draw();

	++m_count;
}