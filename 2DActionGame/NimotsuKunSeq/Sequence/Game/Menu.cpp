#include "Menu.h"
#include "State.h"
#include "Gamebase.h"
#include "Base/Image.h"
#include "GameLib/Framework.h"

using namespace GameLib;

Menu::Menu() : m_pImage(nullptr)
{
	m_pImage = new Image("assets/menu.dds");
}

Menu::~Menu()
{
	SAFE_DELETE(m_pImage);
}

void Menu::update(GameBase* parent)
{
	int inputNum = 0;
	char numChars[] = { '0', '1', '2', '3', '4'};

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
	case 1: // ����
		parent->state()->reset();
		parent->moveTo(GameBase::SEQ_PLAY);
		break;
	case 2: // �ؿ�
		parent->moveTo(GameBase::SEQ_STATE_SELECT);
		break;
	case 3: // ������
		parent->moveTo(GameBase::SEQ_TITLE);
		break;
	case 4: // ����
		parent->moveTo(GameBase::SEQ_PLAY);
		break;
	default:
		break;
	}

	// �Ȼ�����Ϸ����
	parent->state()->draw();

	// �������ٻ�����ף��Ϣ
	m_pImage->draw();
}