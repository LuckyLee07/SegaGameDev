#include "StateSelect.h"
#include "Parent.h"
#include "Base/Image.h"
#include "GameLib/Framework.h"

using namespace GameLib;

StateSelect::StateSelect() : m_pImage(nullptr)
{
	m_pImage = new Image("assets/stageSelect.dds");
}

StateSelect::~StateSelect()
{
	SAFE_DELETE(m_pImage);
}

void StateSelect::update(Parent* parent)
{
	int stateId = 0;
	char numChars[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
	for (int index = 0; index < 10; ++index)
	{
		if (Framework::instance().isKeyTriggered(numChars[index]))
		{
			stateId = index;
		}
		if (stateId > 0)
		{
			parent->setStateId(stateId);
			parent->moveTo(Parent::SEQ_GAME);
			break;
		}
	}
	m_pImage->draw();
}