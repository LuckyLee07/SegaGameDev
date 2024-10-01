#include "Title.h"
#include "Parent.h"
#include "Base/Image.h"
#include "GameLib/Framework.h"

using namespace GameLib;

Title::Title() : m_pImage(nullptr)
{
	m_pImage = new Image("assets/title.dds");
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
	m_pImage->draw();
}