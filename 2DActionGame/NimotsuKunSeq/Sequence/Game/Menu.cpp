#include "Menu.h"
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
	if (Framework::instance().isKeyTriggered(' '))
	{
		parent->moveTo(GameBase::SEQ_STATE_SELECT);
	}
	m_pImage->draw();
}